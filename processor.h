#pragma once

#include <QObject>
#include <QThread>
#include <QVideoFrame>

#include "frameprober.h"
#include "classifier.h"

class Processor : public QObject
{
    Q_OBJECT
public:
    Processor(FrameProbe *probe_, QObject *parent = 0);

signals:
    void processingStarted();
    void processingFinished();

public slots:
    void receiveFrame(const QVideoFrame &frame);

private:
    Classifier *cl_;
    FrameProbe *probe_;

    int i = 0;

};
