#pragma once

#include <opencv/cv.hpp>
#include <QImage>
#include <QVideoFrame>

class Util
{
public:
    static cv::Mat convertToMat(const QImage &image);
    static cv::Mat convertToMat(const QVideoFrame &frame);
    static QImage convertToImage(const cv::Mat &mat);
    static QImage convertToImage(const QVideoFrame &frame);
};
