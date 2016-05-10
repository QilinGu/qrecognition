#include "capture.h"

#include <QDebug>
#include <QTimerEvent>

using namespace std;

Capture::Capture(QObject *parent) : QObject(parent)
{

}

void Capture::startImage(const QString &file) {
    if (!vcapture_) {
        vcapture_.reset(new cv::VideoCapture(file.toStdString()));
    }
    if (vcapture_->isOpened()) {
        timer_.start(0, this);
        emit started();
    }
}

void Capture::startVideo(const QString &file) {
    if (!vcapture_) {
        vcapture_.reset(new cv::VideoCapture(file.toStdString()));
    }
    if (vcapture_->isOpened()) {
        timer_.start(0, this);
        emit started();
    }
}

void Capture::startCamera(int cam) {
    if (!vcapture_) {
        vcapture_.reset(new cv::VideoCapture(cam));
    }
    if (vcapture_->isOpened()) {
        timer_.start(0, this);
        emit started();
    }
}

void Capture::stop() {
    timer_.stop();
}

void Capture::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != timer_.timerId()) {
        return;
    }
    cv::Mat frame;
    // blocks until a new frame is ready
    if (!vcapture_->read(frame)) {
        timer_.stop();
        return;
    }

    //TODO: remove then
    if (frame.empty()) qDebug() << "cv::Mat in Capture is null!";

    emit matReady(frame);
}
