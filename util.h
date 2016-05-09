#pragma once

#include <opencv/cv.hpp>
#include <QImage>
#include <QVideoFrame>
#include <QRectF>
#include <QSize>

class Util
{
public:
    static cv::Mat convertToMat(const QImage &image);
    static cv::Mat convertToMat(const QVideoFrame &frame);
    static QImage convertToImage(const cv::Mat &mat);
    static QImage convertToImage(const QVideoFrame &frame);
    static QRectF convertToRectF(const cv::Rect &rect);
    static QSize convertToSize(cv::Size size);
};
