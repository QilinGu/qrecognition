#include "graphicspixmapitemviewer.h"

#include <chrono>
#include <iostream>

#include <QDebug>

GraphicsPixmapItemViewer::GraphicsPixmapItemViewer(QGraphicsView *view,
                                                   QGraphicsPixmapItem *display,
                                                   QObject *parent)
    : AbstractViewer(parent)
    , view_(view)
    , display_(display)
{}

void GraphicsPixmapItemViewer::displayImage(const QImage &img) {
    if (!img.isNull()) {

        //TODO: remove then
        auto now = std::chrono::high_resolution_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto val = now_ms.time_since_epoch();
        long dur = val.count();
        std::cout << "frame " << ++i << " presented at " << dur << std::endl;

        display_->setPixmap(QPixmap::fromImage(img));
        display_->show();
    } else {
        qDebug() << "Viewer received null image!";
    }
}

void GraphicsPixmapItemViewer::setOverlay(const QPixmap &pm) {
    //TODO:
}

void GraphicsPixmapItemViewer::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)

    view_->fitInView(display_, Qt::KeepAspectRatio);
}
