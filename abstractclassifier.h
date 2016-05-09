#pragma once

#include <vector>

#include <opencv/cv.hpp>

typedef std::pair<int, float> Prediction;

class AbstractClassifier
{
public:
    /* Classify images @imgs and output @topN predictions with highest accuracy. */
    virtual std::vector<std::vector<Prediction> > classify(const std::vector<cv::Mat> &imgs, int topN) = 0;
};
