#pragma once

#include <vector>

#include <QObject>
#include <QString>

#include "classifier.h"
#include "frameprobevsurface.h"

class Processor : public QObject
{
    Q_OBJECT
public:
    Processor(FrameProbeVSurface *probe, QObject *parent = 0);
    ~Processor();

signals:
//    void processingStarted();
//    void processingFinished();

public slots:
    void receiveFrame(QImage frame);

    void initClassifier(const std::vector<QString> &filePaths);
    void initDetector(const std::vector<QString> &filePaths);

private:
    Classifier *cl_;
    Classifier *dt_;
    FrameProbeVSurface *probe_;


    int i = 0;

    /* TODO: this method is just for scratch */
    void loadLabels(const std::string &labels_file);
};
