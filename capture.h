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
    void matReady(const cv::Mat &frame);

    void durationChanged(int n_frames);
    void positionChanged(int frame);
    void playbackSpeedChanged(int fps);

public slots:
    void startImage(const QString &file);
    void startVideo(const QString &file);
    void startCamera(int cam = 0);

    void stop();
    void changeStatePlaying();
    void changeVideoPos(int to_frame);
    void changePlaybackSpeed(int fps);
    void setDefaultPlaybackSpeed();

private:
    QScopedPointer<cv::VideoCapture> vcapture_;
    QBasicTimer timer_;

    int frame_delay_ms_;

    void timerEvent(QTimerEvent *ev);

};
