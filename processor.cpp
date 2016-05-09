#include "processor.h"

#include <iostream>
#include <chrono>

#include <QImage>

#include "util.h"
#include "classifier.h"
#include "caffenetbuilder.h"

using namespace std;

Processor::Processor(FrameProbeVSurface *probe, QObject *parent)
    : QObject(parent)
    , builder_cl_(new CaffeNetBuilder())
    , builder_dt_(nullptr)
    , cl_(nullptr)
    , dt_(nullptr)
    , probe_(probe)
{

}

Processor::~Processor() {
    if (!cl_) {
        delete cl_;
    }
    if (!dt_) {
        delete dt_;
    }
}

void Processor::initClassifier() {
    //TODO: what to do with labels file?
    cl_ = builder_cl_->buildClassifier();
    if (cl_) {
        //TODO: do logic for handling when there is no classifier (or it is).
    }
}

void Processor::initDetector() {
    dt_ = builder_dt_->buildDetector();
    if (dt_) {
        //TODO: do logic for handling when there is no detector (or it is).
    }
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

// TODO: Do somthing with time benchmarking -- it is useful.
    auto now = std::chrono::high_resolution_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto val = now_ms.time_since_epoch();
    long dur = val.count();

    //TODO: do processing code when there will be output logic.

    cout << "frame     received  at " << dur << endl;
    sleep(1);

    probe_->continueProbing();

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
