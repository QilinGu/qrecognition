#pragma once

#include <vector>
#include <string>

#include <opencv/cv.hpp>

#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QLabel>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QRectF>

#include "abstractoutput.h"

class DrawingOutput : public AbstractOutput
{
public:
    explicit DrawingOutput(QObject *parent = nullptr);
    DrawingOutput(QPen pen, QBrush brush, QObject *parent = nullptr);

    void output(const std::vector<std::pair<int, float> > &predictions) override;
    void output(const std::vector<cv::Rect> &boxes) override;
    void output(const std::vector<cv::Rect> &boxes,
                const std::vector<std::vector<std::pair<int, float> > > &predictions) override;

    void update() override;

    void updateOutputSize(cv::Size overlay_size) override;

private:
    QPixmap overlay_;
    cv::Size overlay_size_;
    QVector<QRectF> boxes_;

    QPen pen_;
    QBrush brush_;
    QPainter p_;

    void resetOverlay(cv::Size overlay_size);

    QRectF toQRectF(const cv::Rect &rect) {
        return QRectF(rect.x, rect.y, rect.width, rect.height);
    }
    QSize toQSize(const cv::Size &size) {
        return QSize(size.width, size.height);
    }

};

