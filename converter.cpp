#include "converter.h"

#include <QDebug>
#include <QTimerEvent>

Converter::Converter(QObject *parent)
    : QObject(parent)
    , process_all_(true)
{
}

void Converter::setProcessAll(bool is_all) {
    process_all_ = is_all;
}

void Converter::processFrame(const cv::Mat &frame) {
    if (process_all_) {
        process(frame);
    } else {
        queue(frame);
    }
}

void Converter::process(cv::Mat frame) {
//    cv::resize(frame, frame, cv::Size(), 0.3, 0.3, cv::INTER_AREA);
    cv::cvtColor(frame, frame, CV_BGR2RGB);
    const QImage image(frame.data, frame.cols, frame.rows, frame.step,
                       QImage::Format_RGB888, &matDeleter, new cv::Mat(frame));

    Q_ASSERT(image.constBits() == frame.data);

    emit imageReady(image);
}

void Converter::queue(const cv::Mat &frame) {
    if (!frame_.empty()) {
        qDebug() << "Converter dropped frame.";
    }
    frame_ = frame;
    if (!timer_.isActive()) {
        timer_.start(0, this);
    }
}

void Converter::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != timer_.timerId()) {
        return;
    }
    process(frame_);
    frame_.release();
    timer_.stop();
}
