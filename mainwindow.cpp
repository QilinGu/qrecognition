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
    , thread1()
    , thread2()
    , thread3()
    , thread4()
    , scene(new QGraphicsScene())
    , capture(new Capture())
    , converter(new Converter())
    , viewer(nullptr)
    , output(new DrawingOutput())
    , proc(new Processor(output))
    , net_dialog(new OpenNetDialog(this))
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<cv::Mat>();
    ui->setupUi(this);

    ui->graphicsView->setScene(scene);
    viewer = new LabelViewer(ui->label);
    converter->setProcessAll(false);

    ui->pushButtonPlay->setEnabled(false);
    ui->sliderSeek->setRange(0, 0);

    connect(ui->pushButtonOpenClassifier, &QPushButton::clicked, proc, &Processor::initClassifier);
    connect(ui->pushButtonOpenDetector, &QPushButton::clicked, proc, &Processor::initDetector);
    connect(ui->pushButtonStartCl, &QPushButton::clicked, proc, &Processor::changeStateProcessing);

    connect(ui->pushButtonOpenVideo, &QPushButton::clicked, this, &MainWindow::setVideo);
    connect(ui->pushButtonOpenImage, &QPushButton::clicked, this, &MainWindow::setImage);
    connect(ui->pushButtonCamera, &QPushButton::clicked, this, &MainWindow::setCamera);
    ui->pushButtonCamera->setEnabled( QCameraInfo::availableCameras().count() > 0 );

    connect(ui->pushButtonPlay, &QPushButton::clicked, capture, &Capture::changeStatePlaying);
    connect(ui->sliderSeek, &QSlider::sliderMoved, capture, &Capture::changeVideoPos);
    connect(ui->sliderSpeed, &QSlider::sliderMoved, capture, &Capture::changePlaybackSpeed);
    connect(ui->pushButtonSetDefSpeed, &QPushButton::clicked, capture, &Capture::setDefaultPlaybackSpeed);
    connect(capture, &Capture::positionChanged, this, &MainWindow::positionChanged);
    connect(capture, &Capture::durationChanged, this, &MainWindow::durationChanged);
    connect(capture, &Capture::playbackSpeedChanged, this, &MainWindow::playbackSpeedChanged);
    ui->pushButtonSetDefSpeed->setEnabled(false);
    ui->sliderSpeed->setRange(1, 121);
    ui->sliderSpeed->setEnabled(false);
    ui->sliderSeek->setEnabled(false);

    qDebug() << "main thread " << QThread::currentThreadId();
//    TODO: remove then: not implemented yet
    ui->pushButtonOpenDetector->setEnabled(false);

    capture->moveToThread(&thread1);
    proc->moveToThread(&thread2);
    converter->moveToThread(&thread3);
    viewer->moveToThread(&thread4);
    thread1.start();
    thread2.start();
    thread3.start();
    thread4.start();

    connect(capture, &Capture::matReady, proc, &Processor::receiveFrame, Qt::QueuedConnection);
    connect(capture, &Capture::matReady, converter, &Converter::processFrame, Qt::QueuedConnection);
    connect(converter, &Converter::imageReady, viewer, &AbstractViewer::displayImage, Qt::QueuedConnection);
    connect(output, &AbstractOutput::outputReady, viewer, &AbstractViewer::setOverlay, Qt::QueuedConnection);

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

    // Not checked
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
    thread1.quit();
    thread2.quit();
    thread3.quit();
    thread4.quit();
    thread1.wait();
    thread2.wait();
    thread3.wait();
    thread4.wait();
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev) {
    if (obj == ui->graphicsView->viewport()) {
        if (ev->type() == QEvent::Scroll || ev->type() == QEvent::Wheel || ev->type() == QEvent::GraphicsSceneWheel) {
            return true;
        }
    }
    return false;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    viewer->resizeEvent(event);
}

void MainWindow::setVideo() {
    QString filename = QFileDialog::getOpenFileName( this, "Open Video",
        QDir::homePath(), "Video Files " + formatsToPatterns(supported_video_formats_) );

    if (!filename.isEmpty()) {
        reset();
        QMetaObject::invokeMethod(capture, "startVideo", Q_ARG(QString, filename));
        ui->pushButtonPlay->setEnabled(true);
        ui->pushButtonSetDefSpeed->setEnabled(true);
        ui->sliderSeek->setEnabled(true);
        ui->sliderSpeed->setEnabled(true);
    }
}

void MainWindow::setCamera() {
    reset();
    auto cameras = QCameraInfo::availableCameras();
    if (cameras.count() == 1) {
        QMetaObject::invokeMethod(capture, "startCamera");
    } else {
//      TODO: dialog to choose camera
        QMetaObject::invokeMethod(capture, "startCamera");
    }
}

void MainWindow::setImage() {
    QString filename = QFileDialog::getOpenFileName( this, "Open Image",
        QDir::homePath(), "Image Files " + formatsToPatterns(supported_img_formats_) );

    if (!filename.isEmpty()) {
        reset();
        proc->setOneshot(true);
        proc->setProcessing(true);
        QMetaObject::invokeMethod(capture, "startImage", Q_ARG(QString, filename));
    }
}

void MainWindow::reset() {
    QMetaObject::invokeMethod(capture, "stop");
    proc->setProcessing(false);
    proc->setOneshot(false);
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
