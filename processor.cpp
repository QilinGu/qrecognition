#include "processor.h"

#include <chrono>

#include <QDebug>
#include <QImage>
#include <QFileDialog>
#include <QDir>

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
    cl_ = builder_cl_->buildClassifier();
    emit classifierOpened( (bool)cl_ );
}

void Processor::initDetector() {
    dt_ = builder_dt_->buildDetector();
    emit detectorOpened( (bool)dt_ );
}

void Processor::openLabels(QWidget *dialogParent) {
    if (cl_) {
        // Need dialogParent because without him there is ugly SIGSEGV by gtk.
        QString filename = QFileDialog::getOpenFileName( dialogParent,
            "Open Labels File", QDir::homePath(), "Text Files (*.txt)");

        if (!filename.isEmpty()) {
            bool res = out_->loadLabels( filename.toStdString(), cl_->getNumOutputClasses() );
            emit labelsOpened(res);
        }
    } else {
        qInfo() << "You should specify classifier first.";
    }
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
            qDebug() << "received from capture";
            process(frame);

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
        detected = cropImages(img, boxes);
    } else {
        detected.push_back(img);
    }

    // If user specified classifier, then classify detected images.
    vector<vector<Prediction> > classified;
    if (cl_) {
       classified = cl_->classify(detected, 5);
    }

    // Setting appropriate output.
    cv::Size s(img.cols, img.rows);
    out_->updateOutputSize(s);
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
//            TODO: case for output when there is no both cl and dt
            qInfo() << "Either classifier or detector were not specified - nothing to do.";
        }
    }

//    TODO: remove then: testing output
//    ++i;
//    boxes.push_back(cv::Rect(i, i, i, i));
//    out_->output(boxes);
}

const vector<cv::Mat> Processor::cropImages(const cv::Mat &img, const std::vector<cv::Rect> &boxes) {
    vector<cv::Mat> cropped_imgs;

    for (cv::Rect box : boxes) {
        cropped_imgs.push_back( cv::Mat(img, box) );
    }

    return vector<cv::Mat>();
}
