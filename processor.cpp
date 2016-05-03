#include "processor.h"

#include <iostream>

using namespace std;

Processor::Processor(FrameProbe *probe, QObject *parent)
    : QObject(parent)
    , probe_(probe)
{

}

void Processor::receiveFrame(const QVideoFrame &frame) {

    probe_->stopProbing();
//    emit processingStarted();

// There is a processing code.
//    sleep(1);
//    cout << "frame processd " << i++ << " " << frame.startTime() << " processed from thread: " << QThread::currentThreadId() << endl;

    probe_->continueProbing();
//    emit processingFinished();

}

/* Pipeline :
 *
 * QVideoFrame
 * |> to cv::Mat
 * |> Detect
 * |> Classify(batch)
 * |> OutputResult
 *
 */
