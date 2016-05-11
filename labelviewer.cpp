#include "labelviewer.h"

#include <chrono>
#include <iostream>

#include <QDebug>

LabelViewer::LabelViewer(QLabel *display, QObject *parent)
    : AbstractViewer(parent)
    , bg_display_(display)
    , fg_display_(new QLabel(display))
{
    fg_display_->setAlignment(bg_display_->alignment());
    fg_display_->setGeometry(0, 0, bg_display_->width(), bg_display_->height());
}

void LabelViewer::displayImage(const QImage &img) {
    if (!img.isNull()) {

        //TODO: remove then
        auto now = std::chrono::high_resolution_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto val = now_ms.time_since_epoch();
        long dur = val.count();
        std::cout << "frame presented at " << dur << std::endl;

        QPixmap bg = QPixmap::fromImage(img).scaled(bg_display_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        bg_display_->setPixmap(bg);

    } else {
        qDebug() << "Viewer received null image!";
    }
}

void LabelViewer::setOverlay(const QPixmap &pm) {
    overlay_ = pm;
    fg_display_->setPixmap( pm.scaled(bg_display_->pixmap()->size(), Qt::KeepAspectRatio, Qt::FastTransformation) );
    fg_display_->setGeometry(0, 0, bg_display_->width(), bg_display_->height());

    // TODO: remove then
//    qDebug() << "size of orig overl : " << pm.size().width() << " : " << pm.size().height();
//    qDebug() << "size of fg pixmap  : " << fg_display_->pixmap()->size().width() << " : " << fg_display_->pixmap()->size().height();
//    qDebug() << "size of bg pixmap  : " << bg_display_->pixmap()->size().width() << " : " << bg_display_->pixmap()->size().height();
//    qDebug() << "size of fg_display : " << fg_display_->size().width() << " : " << fg_display_->size().height();
//    qDebug() << "size of bg_display : " << bg_display_->size().width() << " : " << bg_display_->size().height();
}

void LabelViewer::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)

    auto bg = bg_display_->pixmap();
    if (bg) {
        bg_display_->setPixmap( bg->scaled(bg_display_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
        fg_display_->setGeometry(0, 0, bg_display_->width(), bg_display_->height());
        fg_display_->setPixmap( overlay_.scaled(bg_display_->pixmap()->size(), Qt::KeepAspectRatio, Qt::FastTransformation) );
    }
}
