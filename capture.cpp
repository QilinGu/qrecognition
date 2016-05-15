#include "capture.h"

#include <QDebug>
#include <QTimerEvent>

using namespace std;

Capture::Capture(QObject *parent)
    : QObject(parent)
    , frame_delay_ms_(0)
{

}

void Capture::startImage(const QString &file) {
    vcapture_.reset(new cv::VideoCapture(file.toStdString()));

    if (vcapture_->isOpened()) {
        timer_.start(0, this);
    }
}

void Capture::startVideo(const QString &file) {
    vcapture_.reset(new cv::VideoCapture(file.toStdString()));

    int fps = round( vcapture_->get(cv::CAP_PROP_FPS) );
    frame_delay_ms_ = 1000 / fps;
    emit playbackSpeedChanged(fps);
    emit durationChanged( vcapture_->get(cv::CAP_PROP_FRAME_COUNT) );

    if (vcapture_->isOpened()) {
        timer_.start(frame_delay_ms_, this);
    }
}

void Capture::startCamera(int cam) {
    vcapture_.reset(new cv::VideoCapture(cam));

    if (vcapture_->isOpened()) {
        timer_.start(0, this);
    }
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

//        string file = "/home/gkirg/projects/compvis/analyze/own_data/00001.ppm";
//        cv::Mat direct = cv::imread(file);
//        auto diff = (frame != direct);
//        auto n_diff = cv::countNonZero(diff);
//        qDebug() << "diff between captured frame and orig direct:" << n_diff;

    emit matReady(frame);
    emit positionChanged( vcapture_->get(cv::CAP_PROP_POS_FRAMES) );
}

void Capture::stop() {
    timer_.stop();
}

void Capture::changeStatePlaying() {
    timer_.isActive() ? timer_.stop() : timer_.start(frame_delay_ms_, this);
}

void Capture::changeVideoPos(int to_frame) {
    vcapture_->set(cv::CAP_PROP_POS_FRAMES, (double)to_frame);
}

void Capture::changePlaybackSpeed(int fps) {
    timer_.stop();
    if (fps != 0) {
        frame_delay_ms_ = round(1000.0 / fps);
        timer_.start(frame_delay_ms_, this);
        emit playbackSpeedChanged(fps);
    }
}

void Capture::setDefaultPlaybackSpeed() {
    int fps = round( vcapture_->get(cv::CAP_PROP_FPS) );
    timer_.stop();
    if (fps != 0) {
        frame_delay_ms_ = round(1000.0 / fps);
        timer_.start(frame_delay_ms_, this);
    }
    emit playbackSpeedChanged(fps);
}
