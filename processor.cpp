#include "processor.h"

#include <iostream>

using namespace std;

Processor::Processor(FrameProbe *probe, QObject *parent)
    : QObject(parent)
    , cl_(nullptr)
    , dt_(nullptr)
    , probe_(probe)
{

}

Processor::~Processor() {
    delete cl_;
    delete dt_;
}

void Processor::initClassifier(const vector<QString> &filePaths) {
    //Parse filePaths for existence of labels file and mean image file
    //TODO: process labels file
    cl_ = new Classifier(
            filePaths[0].toStdString(),
            filePaths[1].toStdString(),
            filePaths[2].toStdString());
}

void Processor::initDetector(const vector<QString> &filePaths) {
    dt_ = new Classifier(
            filePaths[0].toStdString(),
            filePaths[1].toStdString(),
            filePaths[2].toStdString());
}

void Processor::loadLabels(const string &labels_file) {
    vector<string> labels;

    /* Load labels. */
    ifstream labels(labels_file.c_str());
    CHECK(labels) << "Unable to open labels file " << labels_file;
    string line;
    while (getline(labels, line))
        labels_.push_back(string(line));

    Blob<float>* output_layer = net_->output_blobs()[0];
    CHECK_EQ(labels_.size(), output_layer->channels())
        << "Number of labels is different from the output layer dimension.";
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

//void Processor::pipeline() {
//    if (isDetecting) {
//        // res = dt_->Detect(data);
//        if (isClassifying) {
//            // res' = cl_->Classify(res);
//        } else {
//        }
//    } else {

//    }
//}

/* Pipeline :
 *
 * QVideoFrame
 * |> to cv::Mat
 * |> Detect
 * |> Classify(batch)
 * |> OutputResult
 *
 */
