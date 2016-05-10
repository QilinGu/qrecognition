#include "drawingoutput.h"

#include <iostream>

#include <QColor>

#include "util.h"

using namespace std;

DrawingOutput::DrawingOutput(QGraphicsView *view, QGraphicsPixmapItem *display)
    : view_(view)
    , vitem_(display)
    , orig_img_size_()
    , boxes_()
{
    pen_.setColor(QColor("lime"));
    pen_.setWidth(3);
    pen_.setStyle(Qt::SolidLine);
    brush_.setStyle(Qt::NoBrush);
}

DrawingOutput::DrawingOutput(QGraphicsView *view, QGraphicsPixmapItem *display, QPen pen, QBrush brush)
    : view_(view)
    , vitem_(display)
    , orig_img_size_()
    , boxes_()
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
        boxes_.push_back( view_->scene()->addRect( Util::convertToRectF(box), pen_, brush_ ) );
    }
}

void DrawingOutput::output(const std::vector<cv::Rect> &boxes,
                           const std::vector<std::vector<std::pair<int, float> > > &predictions) {

    Q_ASSERT(boxes.size() == predictions.size());

    boxes_.clear();
    for (cv::Rect box : boxes) {
        boxes_.push_back( view_->scene()->addRect( Util::convertToRectF(box), pen_, brush_ ) );

        //TODO: draw predictions
    }
}

void DrawingOutput::update(cv::Size new_img_size) {
    this->update(orig_img_size_, new_img_size);
}

void DrawingOutput::update(cv::Size orig_img_size, cv::Size new_img_size) {
    if (orig_img_size != cv::Size()) {
        orig_img_size_ = orig_img_size;

        qreal x_coef = (float)new_img_size.width / (float)orig_img_size.width;
        qreal y_coef = (float)new_img_size.height / (float)orig_img_size.height;

        // TODO: pos change for boxes
        for (QGraphicsRectItem *box : boxes_) {

            auto r = box->rect();
            qreal new_w = r.width() * x_coef;
            qreal new_h = r.height() * y_coef;

            box->setRect(r.x(), r.y(), new_w, new_h);

//            QPointF new_pos = box->scenePos() +
//            box->setPos(box->scenePos());
        }
    }
}
