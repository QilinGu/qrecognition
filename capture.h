#pragma once

#include <QObject>
#include <QString>
#include <QBasicTimer>

#include <opencv2/opencv.hpp>

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
    /* Process image.
     * @param file path to image file. */
    void startImage(const QString &file);

    /* Start processing video.
     * @param file path to video file. */
    void startVideo(const QString &file);

    /* Start processing video stream from camera.
     * @param cam camera id (if 0 then choose the default camera. */
    void startCamera(int cam = 0);

    /* Stop processing. */
    void stop();

    /* Change state of processing: if processing, then stop and otherwise. */
    void changeStatePlaying();

    /* Change position of processing the video: set it to particular frame.
     * @param to_frame frame to which set position. */
    void changeVideoPos(int to_frame);

    /* Change the speed of video processing.
     * @param fps frames per second. */
    void changePlaybackSpeed(int fps);

    /* Set processing speed to default value. */
    void setDefaultPlaybackSpeed();

private:
    QScopedPointer<cv::VideoCapture> vcapture_;
    QBasicTimer timer_;

    int frame_delay_ms_;

    void timerEvent(QTimerEvent *ev);

};
