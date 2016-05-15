#pragma once

#include <vector>

#include <QObject>
#include <QMutex>
#include <QReadWriteLock>

#include <opencv2/opencv.hpp>

#include "abstractclassifierbuilder.h"
#include "abstractdetectorbuilder.h"
#include "abstractclassifier.h"
#include "abstractdetector.h"
#include "abstractoutput.h"

class Processor : public QObject
{
    Q_OBJECT
public:
    Processor(AbstractOutput *output, QObject *parent = 0);
    ~Processor();

signals:
    void classifierOpened(bool opened);
    void detectorOpened(bool opened);
    void labelsOpened(bool opened);

public slots:
    /* Receive frame to decide what to do with it.
     * If the previous frame was processed already, then process the new one.
     * Otherwise drop it.
     * @param frame next frame to process. */
    void receiveFrame(const cv::Mat &frame);

    /* Change state of the processing. */
    void changeStateProcessing();

    /* Set state of the processing.
     * @param is_processing set state to this value. */
    void setProcessing(bool is_processing);

    /* Set mode of the processing.
     * One-shot mode or permanent processing.
     * Former needed for images, latter for streams of data (video).
     * @param is_oneshot mode selector. */
    void setOneshot(bool is_oneshot);

    /* Initialise classifier. */
    void initClassifier();

    /* Initialise detector. */
    void initDetector();

    /* Open labels file for more pleasant output. */
    void openLabels(QWidget *dialogParent);

private:
    AbstractClassifierBuilder *builder_cl_;
    AbstractDetectorBuilder *builder_dt_;
    AbstractClassifier *cl_;
    AbstractDetector *dt_;
    AbstractOutput *out_;

    bool is_ready_;
    bool is_processing_;
    bool is_oneshot_;

    QReadWriteLock mutex_;

    //TODO: remove then: for debugging
    int i = 0;

    void process(const cv::Mat &img);
    const std::vector<cv::Mat> cropImages(const cv::Mat &img, const std::vector<cv::Rect> &boxes);
};
