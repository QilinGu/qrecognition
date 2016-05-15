#pragma once

#include <vector>
#include <string>
#include <memory>

#include <opencv/cv.hpp>
#include <caffe/caffe.hpp>

#include <abstractclassifier.h>

typedef std::pair<int, float> Prediction;

class Classifier : public AbstractClassifier
{
public:
    Classifier(
            const std::string &model_file,
            const std::string &weights_file,
            const std::string &mean_img_file = "");

    /* Classify images @imgs and output @topN predictions with highest accuracy. */
    std::vector<std::vector<Prediction> > classify(const std::vector<cv::Mat> &imgs, int topN = 5) override;

    int getNumOutputClasses() override;

private:
    std::shared_ptr<caffe::Net<float> > net_;
    int num_channels_;
    int max_batch_size_;
    cv::Size input_geometry_;
    cv::Mat mean_;

    void setMean(const std::string &mean_file);

    /* Return the indices of the top N values of vector v. */
    std::vector<int> argmax(const std::vector<float> &v, int N);

    std::vector<std::vector<float> > predict(const std::vector<cv::Mat> &imgs);

    /* Wrap the input layer of the network in separate cv::Mat objects
     * (one per channel). This way we save one memcpy operation and we
     * don't need to rely on cudaMemcpy2D. The last preprocessing
     * operation will write the separate channels directly to the input
     * layer. */
    void wrapInputLayer(std::vector<cv::Mat> *input_channels, int batch_size);

    /* Convert the input image to the input image format of the network. */
    void preprocess(const std::vector<cv::Mat> &imgs, std::vector<cv::Mat> *input_channels);
};
