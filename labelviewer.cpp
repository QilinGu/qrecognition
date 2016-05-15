#include "labelviewer.h"

#include <chrono>

#include <QThread>
#include <QDebug>

LabelViewer::LabelViewer(QLabel *display, QObject *parent)
    : AbstractViewer(parent)
    , bg_display_(display)
    , fg_display_(new QLabel(display))
{
    fg_display_->setAlignment(bg_display_->alignment());
    fg_display_->setGeometry(0, 0, bg_display_->width(), bg_display_->height());

    connect(this, &LabelViewer::bg_ready, bg_display_, &QLabel::setPixmap);
    connect(this, &LabelViewer::fg_ready, fg_display_, &QLabel::setPixmap);
}

void LabelViewer::displayImage(const QImage &img) {
    if (!img.isNull()) {

        //TODO: remove then
        auto now = std::chrono::high_resolution_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto val = now_ms.time_since_epoch();
        long dur = val.count();
//        qDebug() << "Viewer: frame presented at" << dur;

        image_ = QPixmap::fromImage(img);
        emit bg_ready( image_.scaled(bg_display_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );

    } else {
        qDebug() << "Viewer: received null image!";
    }
}

void LabelViewer::setOverlay(const QPixmap &pm) {
    overlay_ = pm;

    Q_ASSERT(!pm.paintingActive());
    if (overlay_.isNull()) qDebug() << "Viewer: received null overlay pixmap!";

    fg_display_->setGeometry(0, 0, bg_display_->width(), bg_display_->height());
    emit fg_ready( overlay_.scaled(bg_display_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
}

void LabelViewer::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)

    if (!image_.isNull()) {
        emit bg_ready( image_.scaled(bg_display_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );

        if (!overlay_.isNull()) {
            fg_display_->setGeometry(0, 0, bg_display_->width(), bg_display_->height());
            emit fg_ready( overlay_.scaled(bg_display_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
        }
    }
}
