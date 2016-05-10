#include "abstractdetector.h"


std::vector<cv::Rect> AbstractDetector::detect(const cv::Mat &img) {
    std::vector<cv::Mat> imgs;
    imgs.push_back(img);
    return (this->detect(imgs))[0];
}

AbstractDetector::~AbstractDetector() {}
