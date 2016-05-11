#include "drawingoutput.h"

#include <iostream>

#include <QDebug>
#include <QColor>

using namespace std;

DrawingOutput::DrawingOutput(QObject *parent)
    : AbstractOutput(parent)
    , pen_(Qt::red, 3)
{
    pen_.setStyle(Qt::SolidLine);

    //TODO: remove then: test
    boxes_.push_back(QRectF(30, 30, 100, 200));

}

DrawingOutput::DrawingOutput(QPen pen, QBrush brush, QObject *parent)
    : AbstractOutput(parent)
    , pen_(pen)
    , brush_(brush)
{
}

//TODO: output only predictions
void DrawingOutput::output(const std::vector<std::pair<int, float> > &predictions) {
    if (!is_labels_) {
        for (auto pred : predictions) {
            cout << pred.first << " : probability " << pred.second << endl;
        }
    } else {
        for (auto pred : predictions) {
            cout << labels_[pred.first] << " : probability " << pred.second << endl;
        }
    }
}

void DrawingOutput::output(const std::vector<cv::Rect> &boxes) {
    boxes_.clear();
    for (cv::Rect box : boxes) {
        boxes_.push_back(toQRectF(box));
    }
}

//TODO: draw predictions
void DrawingOutput::output(const std::vector<cv::Rect> &boxes,
                           const std::vector<std::vector<std::pair<int, float> > > &predictions) {

    Q_ASSERT(boxes.size() == predictions.size());

    boxes_.clear();
    for (cv::Rect box : boxes) {
        boxes_.push_back(toQRectF(box));

    }
}

void DrawingOutput::update() {
    update(cv::Size());
}

void DrawingOutput::update(cv::Size orig_img_size) {
    if (orig_img_size != cv::Size()) {
        if (orig_img_size != orig_img_size_) {
            setOrigSize(orig_img_size);
        }

        p_.drawRects(boxes_);

        emit outputReady(output_pm_);
    }
}

void DrawingOutput::setOrigSize(cv::Size orig_img_size) {
    orig_img_size_ = orig_img_size;
    QSize orig_size = toQSize(orig_img_size_);
    output_pm_ = QPixmap(orig_size);
    output_pm_.fill(Qt::transparent);

    p_.begin(&output_pm_);
    p_.setPen(pen_);
    p_.setBrush(brush_);
}

QRectF DrawingOutput::toQRectF(const cv::Rect &rect) {
    return QRectF(rect.x, rect.y, rect.width, rect.height);
}

QSize DrawingOutput::toQSize(const cv::Size &size) {
    return QSize(size.width, size.height);
}
