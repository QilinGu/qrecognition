#include "classifier.h"

#include <opencv/cv.hpp>
#include <opencv/highgui.h>
//#include <opencv/cvwimage.h>

#include <algorithm>
#include <iosfwd>
#include <memory>

using namespace caffe;
using namespace std;

Classifier::Classifier(
            const string &model_file,
            const string &weights_file,
            const string &mean_img_file)
    : max_batch_size_(12)
{
#ifdef CPU_ONLY
    Caffe::set_mode(Caffe::CPU);
#else
    Caffe::set_mode(Caffe::GPU);
#endif

    /* Load the network. */
    net_.reset(new Net<float>(model_file, TEST));
    net_->CopyTrainedLayersFrom(weights_file);

    CHECK_EQ(net_->num_inputs(), 1) << "Network should have exactly one input.";
    CHECK_EQ(net_->num_outputs(), 1) << "Network should have exactly one output.";

    Blob<float>* input_layer = net_->input_blobs()[0];
    num_channels_ = input_layer->channels();
    CHECK(num_channels_ == 3 || num_channels_ == 1) << "Input layer should have 1 or 3 channels.";
    input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

    if (!mean_img_file.empty()) {
        /* Load the binaryproto mean file. */
        SetMean(mean_img_file);
    } else {
        //TODO: test it
        mean_ = cv::Mat::zeros(input_geometry_, CV_64F);
    }
}

void Classifier::SetMean(const string &mean_file) {
    BlobProto blob_proto;
    ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);

    /* Convert from BlobProto to Blob<float> */
    Blob<float> mean_blob;
    mean_blob.FromProto(blob_proto);
    CHECK_EQ(mean_blob.channels(), num_channels_) << "Number of channels of mean file doesn't match input layer.";

    /* The format of the mean file is planar 32-bit float BGR or grayscale. */
    vector<cv::Mat> channels;
    float* data = mean_blob.mutable_cpu_data();
    for (int i = 0; i < num_channels_; ++i) {
        /* Extract an individual channel. */
        cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
        channels.push_back(channel);
        data += mean_blob.height() * mean_blob.width();
    }

    /* Merge the separate channels into a single image. */
    cv::Mat mean;
    cv::merge(channels, mean);

    /* Compute the global mean pixel value and create a mean image
     * filled with this value. */
    cv::Scalar channel_mean = cv::mean(mean);
    mean_ = cv::Mat(input_geometry_, mean.type(), channel_mean);
}

vector<vector<Prediction> > Classifier::Classify(const vector<cv::Mat> &imgs, int topN) {
    auto output = Predict(imgs);
    vector<vector<Prediction> > predictions;
    //TODO: check output.size
//    net_->output_blobs()[0]->channels();
    topN = min<int>(output[0].size(), topN);

    for (unsigned int j = 0; j < output.size(); ++j) {
        vector<int> maxN_j = Argmax(output[j], topN);
        predictions.push_back(vector<Prediction>());

        for (int i = 0; i < topN; ++i) {
            int idx = maxN_j[i];
            predictions[j].push_back(make_pair(idx, output[j][idx]));
        }
    }
    return predictions;
}

vector<vector<float> > Classifier::Predict(const vector<cv::Mat> &imgs) {
    Blob<float>* input_layer = net_->input_blobs()[0];

    //TODO: case when batch > max_batch
    int batch_size = min<int>(imgs.size(), max_batch_size_);

    input_layer->Reshape(batch_size, num_channels_, input_geometry_.height, input_geometry_.width);

    /* Forward dimension change to all layers. */
    net_->Reshape();

    vector<cv::Mat> input_channels;
    WrapInputLayer(&input_channels, batch_size);
    Preprocess(imgs, &input_channels);

//    net_->Forward();
    net_->Forward(net_->input_blobs());

    /* Copy the output layer to a vector of vectors */
    Blob<float>* output_layer = net_->output_blobs()[0];
    vector<vector<float> > ret;
    for (int i = 0; i < batch_size; ++i) {
        const float* begin = output_layer->cpu_data() + i * output_layer->channels();
        const float* end = begin + output_layer->channels();
        ret.push_back(vector<float>(begin, end));
    }
    return ret;
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void Classifier::WrapInputLayer(vector<cv::Mat>* input_channels, int batch_size) {
    Blob<float> *input_layer = net_->input_blobs()[0];

    int width = input_layer->width();
    int height = input_layer->height();
    float *input_data = input_layer->mutable_cpu_data();
    for (int i = 0; i < input_layer->channels() * batch_size; ++i) {
        cv::Mat channel(height, width, CV_32FC1, input_data);
        input_channels->push_back(channel);
        input_data += width * height;
    }
}

/* Convert the input image to the input image format of the network. */
void Classifier::Preprocess(const vector<cv::Mat> &imgs, vector<cv::Mat> *input_channels) {
    for (unsigned int i = 0; i < imgs.size(); ++i) {
        cv::Mat sample;

        if (imgs[i].channels() == 3 && num_channels_ == 1)
            cv::cvtColor(imgs[i], sample, cv::COLOR_BGR2GRAY);
        else if (imgs[i].channels() == 4 && num_channels_ == 1)
            cv::cvtColor(imgs[i], sample, cv::COLOR_BGRA2GRAY);
        else if (imgs[i].channels() == 4 && num_channels_ == 3)
            cv::cvtColor(imgs[i], sample, cv::COLOR_BGRA2BGR);
        else if (imgs[i].channels() == 1 && num_channels_ == 3)
            cv::cvtColor(imgs[i], sample, cv::COLOR_GRAY2BGR);
        else
            sample = imgs[i];

        cv::Mat sample_resized;
        if (sample.size() != input_geometry_)
            cv::resize(sample, sample_resized, input_geometry_);
        else
            sample_resized = sample;

        cv::Mat sample_float;
        if (num_channels_ == 3)
            sample_resized.convertTo(sample_float, CV_32FC3);
        else
            sample_resized.convertTo(sample_float, CV_32FC1);

        cv::Mat sample_normalized;
        cv::subtract(sample_float, mean_, sample_normalized);

        /* This operation will write the separate BGR planes directly to the
         * input layer of the network because it is wrapped by the cv::Mat
         * objects in input_channels. */
        //cv::split(sample_normalized, (*input_channels)[i * num_channels_]);

        vector<cv::Mat> channels;
        cv::split(sample_normalized, channels);
        for (unsigned int j = 0; j < channels.size(); ++j){
             channels[j].copyTo( (*input_channels)[i * num_channels_ + j] );
        }

        //CHECK(reinterpret_cast<float *>(input_channels->at(i).data) == net_->input_blobs()[0]->cpu_data())
        //    << "Input channels are not wrapping the input layer of the network.";
    }
}

/* Return the indices of the top N values of vector v. */
vector<int> Classifier::Argmax(const vector<float> &v, int N) {
    vector<pair<float, int>> pairs;
    for (size_t i = 0; i < v.size(); ++i)
        pairs.push_back(make_pair(v[i], i));

    auto pairCompare = [] (const pair<float, int> &lhs, const pair<float, int> &rhs) {
        return lhs.first > rhs.first;
    };
    partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), pairCompare);

    vector<int> result;
    for (int i = 0; i < N; ++i)
        result.push_back(pairs[i].second);
    return result;
}
