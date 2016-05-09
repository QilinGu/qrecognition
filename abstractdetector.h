#pragma once

#include <vector>

#include <opencv/cv.hpp>

class AbstractDetector
{
public:
    /* Detect objects on the @img and return bounding boxes for them. */
    virtual std::vector<cv::Rect> detect(const cv::Mat &img) = 0;
};
