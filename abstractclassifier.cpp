#include "abstractclassifier.h"

std::vector<Prediction> AbstractClassifier::classify(const cv::Mat &img, int topN) {
    std::vector<cv::Mat> imgs;
    imgs.push_back(img);
    return (this->classify(imgs))[0];
}

AbstractClassifier::~AbstractClassifier() {}
