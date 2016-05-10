#pragma once

#include <QObject>
#include <QString>
#include <QBasicTimer>

#include <opencv2/opencv.hpp>
//#include <opencv/cv.hpp>

class Capture : public QObject
{
    Q_OBJECT
public:
    explicit Capture(QObject *parent = 0);

signals:
    void started();
    void matReady(const cv::Mat &frame);

public slots:
    void startImage(const QString &file);
    void startVideo(const QString &file);
    void startCamera(int cam = 0);
    void stop();

private:
    QScopedPointer<cv::VideoCapture> vcapture_;
    QBasicTimer timer_;

    void timerEvent(QTimerEvent *ev);

};
