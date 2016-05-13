#include "drawingoutput.h"

#include <iostream>

#include <QDebug>
#include <QColor>

using namespace std;

DrawingOutput::DrawingOutput(QObject *parent)
    : AbstractOutput(parent)
    , pen_(Qt::red, 2)
{
    pen_.setStyle(Qt::SolidLine);
}

DrawingOutput::DrawingOutput(QPen pen, QBrush brush, QObject *parent)
    : AbstractOutput(parent)
    , pen_(pen)
    , brush_(brush)
{
    //TODO: don't forget this constructor
}

//TODO: output only predictions
void DrawingOutput::setOutput(const std::vector<std::pair<int, float> > &predictions) {
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

void DrawingOutput::setOutput(const std::vector<cv::Rect> &boxes) {
    boxes_.clear();
    for (cv::Rect box : boxes) {
        boxes_.push_back(toQRectF(box));
    }
}

//TODO: draw predictions
void DrawingOutput::setOutput(const std::vector<cv::Rect> &boxes,
                              const std::vector<std::vector<std::pair<int, float> > > &predictions) {

    Q_ASSERT(boxes.size() == predictions.size());

    boxes_.clear();
    for (cv::Rect box : boxes) {
        boxes_.push_back(toQRectF(box));

    }
}

void DrawingOutput::update() {
    update(orig_img_size_);
}

void DrawingOutput::update(cv::Size orig_img_size) {
    if (orig_img_size != cv::Size()) {
        if (orig_img_size_ != orig_img_size) {
            resetEmptyOverlay(orig_img_size);
        }

        Q_ASSERT(!overlay_.isNull());

        p_.fillRect(overlay_.rect(), Qt::transparent);
        p_.drawRects(boxes_);

//        emit outputReady( overlay_.copy(overlay_.rect()) );
        emit outputReady( overlay_ );
    }
}

void DrawingOutput::resetEmptyOverlay(cv::Size orig_img_size) {
    orig_img_size_ = orig_img_size;
    overlay_ = QPixmap( toQSize(orig_img_size_) );
    if (!p_.isActive()) {
        overlay_.fill(Qt::transparent);
        p_.begin(&overlay_);
        p_.setPen(pen_);
        p_.setBrush(brush_);
        p_.setCompositionMode(QPainter::CompositionMode_Source);
    }

    Q_ASSERT(overlay_.paintingActive());
}
