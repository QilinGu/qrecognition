#pragma once

#include <vector>

#include <QObject>

#include "abstractclassifierbuilder.h"
#include "abstractdetectorbuilder.h"
#include "abstractclassifier.h"
#include "abstractdetector.h"
#include "abstractoutput.h"
#include "frameprobevsurface.h"

class Processor : public QObject
{
    Q_OBJECT
public:
    Processor(AbstractOutput *output, FrameProbeVSurface *probe, QObject *parent = 0);
    ~Processor();

public slots:
    void receiveFrame(QImage frame);

    void initClassifier();
    void initDetector();

private:
    AbstractClassifierBuilder *builder_cl_;
    AbstractDetectorBuilder *builder_dt_;
    AbstractClassifier *cl_;
    AbstractDetector *dt_;
    AbstractOutput *out_;
    FrameProbeVSurface *probe_;

    /*dbg*/int i = 0;

    void process(QImage frame);
    std::vector<cv::Mat> cropImages(const std::vector<cv::Rect> &boxes);

    /* TODO: this method is just for scratch */
    void loadLabels(const std::string &labels_file);
};
