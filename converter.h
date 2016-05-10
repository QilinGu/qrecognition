#pragma once

#include <QObject>
#include <QBasicTimer>
#include <QImage>

#include <opencv2/opencv.hpp>

class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter(QObject *parent = 0);

    void setProcessAll(bool is_all);

signals:
    void imageReady(const QImage &img);

public slots:
    void processFrame(const cv::Mat &frame);

private:
    QBasicTimer timer_;
    cv::Mat frame_;
    bool process_all_;

    static void matDeleter(void *mat) {
        delete static_cast<cv::Mat *>(mat);
    }

    void queue(const cv::Mat &frame);
    void process(cv::Mat frame);
    void timerEvent(QTimerEvent *ev);
};
