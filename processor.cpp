#include "processor.h"

#include <iostream>
#include <chrono>

#include <QImage>

using namespace std;

Processor::Processor(FrameProbeVSurface *probe, QObject *parent)
    : QObject(parent)
    , cl_(nullptr)
    , dt_(nullptr)
    , probe_(probe)
{

}

Processor::~Processor() {
    if (!cl_)
        delete cl_;
    if (!dt_)
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
//    vector<string> labels;

//    /* Load labels. */
//    ifstream labels(labels_file.c_str());
//    CHECK(labels) << "Unable to open labels file " << labels_file;
//    string line;
//    while (getline(labels, line))
//        labels_.push_back(string(line));

//    Blob<float>* output_layer = net_->output_blobs()[0];
//    CHECK_EQ(labels_.size(), output_layer->channels())
//        << "Number of labels is different from the output layer dimension.";
}

void Processor::receiveFrame(QImage frame) {

    probe_->pauseProbing();
//    emit processingStarted();

// There is a processing code.
    auto now = std::chrono::high_resolution_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto val = now_ms.time_since_epoch();
    long dur = val.count();

    cout << "frame     received  at " << dur << endl;
    sleep(1);

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
