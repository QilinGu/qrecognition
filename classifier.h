#pragma once

#include <vector>
#include <string>
#include <memory>

#include <opencv/cv.hpp>
#include <caffe/caffe.hpp>

typedef std::pair<int, float> Prediction;

class Classifier
{
public:
    Classifier(
            const std::string &model_file,
            const std::string &weights_file,
            const std::string &mean_img_file,
            const std::string &labels_file);

    std::vector<Prediction> Classify(const cv::Mat &img, int N = 5);

private:
    std::vector<std::string> labels_;
    std::shared_ptr<caffe::Net<float> > net_;
    int num_channels_;
    int batch_size_;
    cv::Size input_geometry_;
    cv::Mat mean_;

    void SetMean(const std::string &mean_file);

    /* Return the indices of the top N values of vector v. */
    std::vector<int> Argmax(const std::vector<float> &v, int N);
    std::vector<float> Predict(const cv::Mat &img);

    /* Convert the input image to the input image format of the network. */
    void Preprocess(const cv::Mat &img, std::vector<cv::Mat> *input_channels);

    /* Wrap the input layer of the network in separate cv::Mat objects
     * (one per channel). This way we save one memcpy operation and we
     * don't need to rely on cudaMemcpy2D. The last preprocessing
     * operation will write the separate channels directly to the input
     * layer. */
    void WrapInputLayer(std::vector<cv::Mat> *input_channels);
};
