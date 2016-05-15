#pragma once

#include <vector>

#include <opencv/cv.hpp>

typedef std::pair<int, float> Prediction;

class AbstractClassifier
{
public:
    /* Classify images img and output topN predictions with highest accuracy.
     * @param img image to classify.
     * @param topN how much predictions return.
     * @return vector of predictions for image. */
    virtual std::vector<Prediction> classify(const cv::Mat &img, int topN = 1);

    /* Classify images imgs and output topN predictions with highest accuracy.
     * @param imgs images to classify.
     * @param topN how much predictions return.
     * @return vector of predictions for every image. */
    virtual std::vector<std::vector<Prediction> > classify(const std::vector<cv::Mat> &imgs, int topN = 1) = 0;

    /* Utility method: returns number of output classes.
     * @return number of output classes. */
    virtual int getNumOutputClasses() = 0;

    virtual ~AbstractClassifier();
};
