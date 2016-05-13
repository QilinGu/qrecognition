#include "processor.h"

#include <iostream>
#include <chrono>

#include <QDebug>
#include <QImage>

#include "classifier.h"
#include "caffenetbuilder.h"

using namespace std;

Processor::Processor(AbstractOutput *output, QObject *parent)
    : QObject(parent)
    , builder_cl_(new CaffeNetBuilder())
    , builder_dt_(nullptr)
    , cl_(nullptr)
    , dt_(nullptr)
    , out_(output)
    , is_ready_(true)
    , is_processing_(false)
    , is_oneshot_(false)
{}

Processor::~Processor() {
    if (!cl_) {
        delete cl_;
    }
    if (!dt_) {
        delete dt_;
    }
    if (!builder_dt_) {
        delete builder_dt_;
    }
    if (!builder_cl_) {
        delete builder_cl_;
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
    Q_UNUSED(labels_file)
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

void Processor::changeStateProcessing() {
    mutex_.lockForWrite();
    is_processing_ = !is_processing_;
    mutex_.unlock();
}

void Processor::setProcessing(bool is_processing) {
    mutex_.lockForWrite();
    is_processing_ = is_processing;
    mutex_.unlock();
}

void Processor::setOneshot(bool is_oneshot) {
    mutex_.lockForWrite();
    is_oneshot_ = is_oneshot;
    mutex_.unlock();
}

void Processor::receiveFrame(const cv::Mat &frame) {

    mutex_.lockForRead();
    if (is_processing_) {
        mutex_.unlock();
        mutex_.lockForWrite();
        if (is_ready_) {
            is_ready_ = false;
            mutex_.unlock();

    //      TODO: Do somthing with time benchmarking -- it is useful.
            auto now = std::chrono::high_resolution_clock::now();
            auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

//            qDebug() << "Processor: frame size: " << frame.cols << " : " << frame.rows;
            process(frame);
//            sleep(1);

            auto now_end = std::chrono::high_resolution_clock::now();
            auto now_end_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now_end);
            auto val = now_end_ms.time_since_epoch() - now_ms.time_since_epoch();
            long dur = val.count();
            qDebug() << "Processor: frame " << ++i << " processed in " << dur << endl;

            mutex_.lockForWrite();
            is_ready_ = true;
            if (is_oneshot_) {
                is_processing_ = false;
            }
            mutex_.unlock();
        }
        mutex_.unlock();
    }
    mutex_.unlock();
}

void Processor::process(const cv::Mat &img) {

    // If user specified detector, then detect objects from image.
    vector<cv::Mat> detected;
    vector<cv::Rect> boxes;
    if (dt_) {
        boxes = dt_->detect(img);
        detected = cropImages(boxes);
    } else {
        detected.push_back(img);
    }

    // If user specified classifier, then classify detected images.
    vector<vector<Prediction> > classified;
    if (cl_) {
       classified = cl_->classify(detected, 5);
    }

    // Prepare output.
    cv::Size s(img.cols, img.rows);
    ++i;
    boxes.push_back(cv::Rect(i, i, i, i));
    out_->setOutput(boxes);
    out_->update(s);

    if (dt_) {
        if (cl_) {
            out_->setOutput(boxes, classified);
        } else {
            out_->setOutput(boxes);
        }
    } else {
        if (cl_) {
            out_->setOutput(classified[0]);
        } else {
            //TODO: case for output when there is no both cl and dt
//            qDebug() << "Processor: either classifier or detecor were not specified - nothing to do.";
        }

    }
}

// TODO: cropImages()
vector<cv::Mat> Processor::cropImages(const std::vector<cv::Rect> &boxes) {
    Q_UNUSED(boxes)

    return vector<cv::Mat>();
}
