#include "labelviewer.h"

#include <chrono>
#include <iostream>

#include <QDebug>

LabelViewer::LabelViewer(QLabel *display, QObject *parent)
    : AbstractViewer(parent)
    , display_(display)
{}

void LabelViewer::displayImage(const QImage &img) {
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

void LabelViewer::resizeEvent(QResizeEvent *event) {
    //TODO: resizeEvent
    Q_UNUSED(event)
}
