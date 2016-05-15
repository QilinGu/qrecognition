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
}

void DrawingOutput::output(const std::vector<std::pair<int, float> > &predictions) {
    QString s;
    if (!is_labels_) {
        for (auto pred : predictions) {
            s.append("class: ");
            s.append(to_string(pred.first).c_str());
            s.append(", prob.: ");
            s.append(QString::number(pred.second, 'f', 3));
            s.append("\n");
        }
    } else {
        for (auto pred : predictions) {
            s.append(QString::number(pred.second, 'f', 3));
            s.append(" : ");
            s.append( labels_[pred.first].c_str() );
            s.append("\n");
        }
    }
    emit outputReady(s);
}

void DrawingOutput::output(const std::vector<cv::Rect> &boxes) {
    boxes_.clear();
    for (cv::Rect box : boxes) {
        boxes_.push_back(toQRectF(box));
    }

    update();
}

//TODO: draw predictions
void DrawingOutput::output(const std::vector<cv::Rect> &boxes,
                              const std::vector<std::vector<std::pair<int, float> > > &predictions) {

    Q_ASSERT(boxes.size() == predictions.size());

    boxes_.clear();
    for (cv::Rect box : boxes) {
        boxes_.push_back(toQRectF(box));
    }

    update();
}

void DrawingOutput::update() {
    if (overlay_size_!= cv::Size()) {
        Q_ASSERT(!overlay_.isNull());

        p_.fillRect(overlay_.rect(), Qt::transparent);
        p_.drawRects(boxes_);

        emit overlayReady(overlay_);
    }
}

void DrawingOutput::updateOutputSize(cv::Size overlay_size) {
    if (overlay_size != overlay_size_) {
        resetOverlay(overlay_size);
    }
}

void DrawingOutput::resetOverlay(cv::Size overlay_size) {
    overlay_size_ = overlay_size;
    p_.end();
    overlay_ = QPixmap( toQSize(overlay_size_) );
    overlay_.fill(Qt::transparent);
    p_.begin(&overlay_);
    p_.setPen(pen_);
    p_.setBrush(brush_);
    p_.setCompositionMode(QPainter::CompositionMode_Source);

    Q_ASSERT(overlay_.paintingActive());
}
