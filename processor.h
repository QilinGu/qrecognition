#pragma once

#include <vector>

#include <QObject>
#include <QString>
#include <QThread>
#include <QVideoFrame>

#include "frameprober.h"
#include "classifier.h"

class Processor : public QObject
{
    Q_OBJECT
public:
    Processor(FrameProbe *probe_, QObject *parent = 0);
    ~Processor();

signals:
    void processingStarted();
    void processingFinished();

public slots:
    void receiveFrame(const QVideoFrame &frame);

    void initClassifier(const std::vector<QString> &filePaths);
    void initDetector(const std::vector<QString> &filePaths);

private:
    Classifier *cl_;
    Classifier *dt_;
    FrameProbe *probe_;


    /* just for scratch */
    void loadLabels(const std::string &labels_file);
};
