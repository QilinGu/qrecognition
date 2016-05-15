#pragma once

#include <vector>

#include <opencv/cv.hpp>

class AbstractDetector
{
public:
    /* Detect objects on the img and return bounding boxes for them.
     * @param img image for detection.
     * @return vector of bounding boxes of detected objects. */
    virtual std::vector<cv::Rect> detect(const cv::Mat &img);

    /* Detect objects on the imgs and return bounding boxes for them.
     * @param imgs images for detection.
     * @return vector of bounding boxes of detected objects for every image. */
    virtual std::vector<std::vector<cv::Rect> > detect(const std::vector<cv::Mat> &imgs) = 0;

    virtual ~AbstractDetector();
};
