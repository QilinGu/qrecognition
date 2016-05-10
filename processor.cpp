#include "processor.h"

#include <iostream>
#include <chrono>

#include <QImage>
#include <QDebug>

#include "util.h"
#include "classifier.h"
#include "caffenetbuilder.h"

using namespace std;

Processor::Processor(AbstractOutput *output, FrameProbeVSurface *probe, QObject *parent)
    : QObject(parent)
    , builder_cl_(new CaffeNetBuilder())
    , builder_dt_(nullptr)
    , cl_(nullptr)
    , dt_(nullptr)
    , out_(output)
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
//    auto val = now_ms.time_since_epoch();
//    long dur = val.count();

    //TODO: do processing code when there will be output logic.
    cout << "frame     received  at " << endl;
//    sleep(1);
    process(frame);

    probe_->continueProbing();

    auto now_end = std::chrono::high_resolution_clock::now();
    auto now_end_ms = std::chrono::time_point_cast<std::chrono::microseconds>(now_end);
    auto val = now_end_ms.time_since_epoch() - now_ms.time_since_epoch();
    long dur = val.count();

    cout << "frame processed in " << dur << endl;

}

void Processor::process(QImage frame) {
    cv::Mat img = Util::convertToMat(frame);

    // if user specified detector, then detect objects from image
    vector<cv::Mat> detected;
    vector<cv::Rect> boxes;
    if (dt_) {
        boxes = dt_->detect(img);
        detected = cropImages(boxes);
    } else {
        detected.push_back(img);
    }

    // if user specified classifier, then classify images
    vector<vector<Prediction> > classified;
    if (cl_) {
       classified = cl_->classify(detected, 5);
    }

    if (dt_) {
        if (cl_) {
            out_->output(boxes, classified);
        } else {
            out_->output(boxes);
        }
    } else {
        if (cl_) {
            out_->output(classified[0]);
        } else {
            //TODO: case for output when there is no both cl and dt
            qDebug() << "Either classifier or detecor were not specifid :"
                     << "nothing to do.";
        }

    }
}

// TODO: cropImages()
vector<cv::Mat> Processor::cropImages(const std::vector<cv::Rect> &boxes) {
    return vector<cv::Mat>();
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
