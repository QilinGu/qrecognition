#pragma once

#include <vector>
#include <string>

#include <opencv/cv.hpp>

#include <QImage>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPen>
#include <QBrush>

#include "abstractoutput.h"

class DrawingOutput : public AbstractOutput
{
public:
    DrawingOutput(QGraphicsView *view, QGraphicsPixmapItem *display);
    DrawingOutput(QGraphicsView *view, QGraphicsPixmapItem *display,
                  QPen pen, QBrush brush);

    void output(const std::vector<std::pair<int, float> > &predictions) override;
    void output(const std::vector<cv::Rect> &boxes) override;
    void output(const std::vector<cv::Rect> &boxes,
                const std::vector<std::vector<std::pair<int, float> > > &predictions) override;

    void update(cv::Size new_img_size) override;
    void update(cv::Size orig_img_size, cv::Size new_img_size) override;

private:
    QGraphicsView *view_;
    QGraphicsPixmapItem *vitem_;
    cv::Size orig_img_size_;
    std::vector<QGraphicsRectItem *> boxes_;

    QPen pen_;
    QBrush brush_;

};
