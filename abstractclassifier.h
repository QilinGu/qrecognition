#pragma once

#include <vector>

#include <opencv/cv.hpp>

typedef std::pair<int, float> Prediction;

class AbstractClassifier
{
public:
    /* Classify image @img and output @topN predictions with highest accuracy. */
    virtual std::vector<Prediction> classify(const cv::Mat &img, int topN = 1);

    /* Classify images @imgs and output @topN predictions with highest accuracy. */
    virtual std::vector<std::vector<Prediction> > classify(const std::vector<cv::Mat> &imgs, int topN = 1) = 0;

    virtual ~AbstractClassifier();
};
