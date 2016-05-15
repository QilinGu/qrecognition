#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaObject>
#include <QDebug>
#include <QUrl>
#include <QDir>
#include <QFileDialog>
#include <QResizeEvent>
#include <QCameraInfo>

#include "opennetdialog.h"
#include "graphicspixmapitemviewer.h"
#include "labelviewer.h"
#include "drawingoutput.h"

Q_DECLARE_METATYPE(cv::Mat)

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , thread1_()
    , thread2_()
    , thread3_()
    , thread4_()
    , capture_(new Capture())
    , converter_(new Converter())
    , viewer_(nullptr)
    , output_(nullptr)
    , proc_(nullptr)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<cv::Mat>();
    ui->setupUi(this);

    output_ = new DrawingOutput();
    proc_ = new Processor(output_);
    viewer_ = new LabelViewer(ui->label);
    converter_->setProcessAll(false);

    connect(ui->pushButtonOpenVideo, &QPushButton::clicked, this, &MainWindow::openVideo);
    connect(ui->pushButtonOpenImage, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(ui->pushButtonCamera, &QPushButton::clicked, this, &MainWindow::openCamera);
    ui->pushButtonCamera->setEnabled( QCameraInfo::availableCameras().count() > 0 );

    connect(ui->pushButtonPlay, &QPushButton::clicked, capture_, &Capture::changeStatePlaying, Qt::QueuedConnection);
    connect(ui->sliderSeek, &QSlider::sliderMoved, capture_, &Capture::changeVideoPos, Qt::QueuedConnection);
    connect(ui->sliderSpeed, &QSlider::sliderMoved, capture_, &Capture::changePlaybackSpeed, Qt::QueuedConnection);
    connect(ui->pushButtonSetDefSpeed, &QPushButton::clicked, capture_, &Capture::setDefaultPlaybackSpeed, Qt::QueuedConnection);
    connect(capture_, &Capture::positionChanged, this, &MainWindow::positionChanged);
    connect(capture_, &Capture::durationChanged, this, &MainWindow::durationChanged);
    connect(capture_, &Capture::playbackSpeedChanged, this, &MainWindow::playbackSpeedChanged);
    ui->pushButtonPlay->setEnabled(false);
    ui->pushButtonSetDefSpeed->setEnabled(false);
    ui->sliderSpeed->setRange(1, 121);
    ui->sliderSpeed->setEnabled(false);
    ui->sliderSeek->setEnabled(false);

    connect(ui->pushButtonOpenClassifier, &QPushButton::clicked, proc_, &Processor::initClassifier);
    connect(ui->pushButtonOpenDetector, &QPushButton::clicked, proc_, &Processor::initDetector);
    connect(ui->pushButtonStartCl, &QPushButton::clicked, proc_, &Processor::changeStateProcessing);
//    connect(ui->pushButtonOpenLabels, &QPushButton::clicked, proc, &Processor::openLabels);
    connect(ui->pushButtonOpenLabels, &QPushButton::clicked, [this](){proc_->openLabels(this);});
    QString statusOn("Loaded");
    QString statusOff("Not Loaded");
    connect(proc_, &Processor::classifierOpened, [this, statusOn, statusOff](bool checked) {
        checked ? ui->lineEditClassifierStatus->setText(statusOn) : ui->lineEditClassifierStatus->setText(statusOff);
    });
    connect(proc_, &Processor::detectorOpened, [this, statusOn, statusOff](bool checked) {
        checked ? ui->lineEditDetectorStatus->setText(statusOn) : ui->lineEditDetectorStatus->setText(statusOff);
    });
    connect(proc_, &Processor::labelsOpened, [this, statusOn, statusOff](bool checked) {
        checked ? ui->lineEditLabelsStatus->setText(statusOn) : ui->lineEditLabelsStatus->setText(statusOff);
    });

    qDebug() << "main thread " << QThread::currentThreadId();
//    TODO: remove then: not implemented yet
    ui->pushButtonOpenDetector->setEnabled(false);

    capture_->moveToThread(&thread1_);
    proc_->moveToThread(&thread2_);
    converter_->moveToThread(&thread3_);
    viewer_->moveToThread(&thread4_);
    thread1_.start();
    thread2_.start();
    thread3_.start();
    thread4_.start();

    connect(capture_, &Capture::matReady, proc_, &Processor::receiveFrame, Qt::QueuedConnection);
    connect(capture_, &Capture::matReady, converter_, &Converter::processFrame, Qt::QueuedConnection);
    connect(converter_, &Converter::imageReady, viewer_, &AbstractViewer::displayImage, Qt::QueuedConnection);
    connect(output_, &AbstractOutput::overlayReady, viewer_, &AbstractViewer::setOverlay, Qt::QueuedConnection);
    connect(output_, &AbstractOutput::outputReady, ui->textEdit, &QTextEdit::append, Qt::QueuedConnection);

    initSupportedFormats();
}

void MainWindow::initSupportedFormats() {
    // Always supported
    supported_img_formats_.push_back("bmp");
    supported_img_formats_.push_back("dib");
    supported_img_formats_.push_back("pbm");
    supported_img_formats_.push_back("pgm");
    supported_img_formats_.push_back("ppm");
    supported_img_formats_.push_back("sr");
    supported_img_formats_.push_back("ras");

    // Depends on codecs presence
    supported_img_formats_.push_back("jpeg");
    supported_img_formats_.push_back("jpg");
    supported_img_formats_.push_back("jp2");
    supported_img_formats_.push_back("png");
    supported_img_formats_.push_back("webp");
    supported_img_formats_.push_back("tif");
    supported_img_formats_.push_back("tiff");

    // Depends on codecs presence
    supported_video_formats_.push_back("mkv");
    supported_video_formats_.push_back("avi");
    supported_video_formats_.push_back("mp4");
    supported_video_formats_.push_back("flv");
    supported_video_formats_.push_back("wmv");
    supported_video_formats_.push_back("mpg");
    supported_video_formats_.push_back("mpeg");
    supported_video_formats_.push_back("m4v");
    supported_video_formats_.push_back("3gp");
}

QString MainWindow::formatsToPatterns(const std::vector<string> &formats) {
    QString patterns("(");
    for (string s : formats) {
        patterns.append("*.");
        patterns.append(s.c_str());
        patterns.append(" ");
    }
    patterns.append( ")" );
    return patterns;
}

MainWindow::~MainWindow()
{
    thread1_.quit();
    thread2_.quit();
    thread3_.quit();
    thread4_.quit();
    thread1_.wait();
    thread2_.wait();
    thread3_.wait();
    thread4_.wait();

    delete capture_;
    delete converter_;
    delete proc_;
    delete output_;
    delete viewer_;

    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    viewer_->resizeEvent(event);
}

void MainWindow::openVideo() {
    QString filename = QFileDialog::getOpenFileName( this, "Open Video",
        QDir::homePath(), "Video Files " + formatsToPatterns(supported_video_formats_) );

    if (!filename.isEmpty()) {
        reset();
        QMetaObject::invokeMethod(capture_, "startVideo", Q_ARG(QString, filename));
        ui->pushButtonPlay->setEnabled(true);
        ui->pushButtonSetDefSpeed->setEnabled(true);
        ui->sliderSeek->setEnabled(true);
        ui->sliderSpeed->setEnabled(true);
    }
}

void MainWindow::openCamera() {
    reset();
    auto cameras = QCameraInfo::availableCameras();
    if (cameras.count() == 1) {
        QMetaObject::invokeMethod(capture_, "startCamera");
    } else {
//      TODO: dialog to choose camera
        QMetaObject::invokeMethod(capture_, "startCamera");
    }
}

void MainWindow::openImage() {
    QString filename = QFileDialog::getOpenFileName( this, "Open Image",
        QDir::homePath(), "Image Files " + formatsToPatterns(supported_img_formats_) );

    if (!filename.isEmpty()) {
        reset();
        proc_->setOneshot(true);
        proc_->setProcessing(true);
        QMetaObject::invokeMethod(capture_, "startImage", Q_ARG(QString, filename));
    }
}

void MainWindow::reset() {
    QMetaObject::invokeMethod(capture_, "stop");
    proc_->setProcessing(false);
    proc_->setOneshot(false);
    ui->pushButtonPlay->setEnabled(false);
    ui->pushButtonSetDefSpeed->setEnabled(false);
    ui->sliderSeek->setEnabled(false);
    ui->sliderSpeed->setEnabled(false);
}

void MainWindow::positionChanged(qint64 position) {
    ui->sliderSeek->setValue(position);
}

void MainWindow::durationChanged(qint64 duration) {
    ui->sliderSeek->setRange(0, duration);
}

void MainWindow::playbackSpeedChanged(qint64 fps) {
    ui->sliderSpeed->setValue(fps);
}
