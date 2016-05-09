#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QUrl>
#include <QFileDialog>
#include <QDir>
#include <QResizeEvent>
#include <QCameraInfo>

#include "opennetdialog.h"
#include "drawingoutput.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , procThread()
    , probThread()
    , player(new QMediaPlayer())
    , camera(nullptr)
    , vitem(new QGraphicsPixmapItem())
    , output(nullptr)
    , ui(new Ui::MainWindow)
    , net_dialog(new OpenNetDialog(this))
    , proc(nullptr)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
//    vitem->setPos(0, 0);
    scene->addItem(vitem);
    output = new DrawingOutput(ui->graphicsView, vitem);
    probe = new FrameProbeVSurface(vitem);
    proc = new Processor(probe);

    ui->pushButtonPlay->setEnabled(false);
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->horizontalSliderSeek->setRange(0, 0);

    connect(ui->pushButtonOpenClassifier, &QPushButton::clicked, proc, &Processor::initClassifier);
    connect(ui->pushButtonOpenDetector, &QPushButton::clicked, proc, &Processor::initDetector);
    connect(ui->pushButtonStartCl, &QPushButton::clicked, probe, &FrameProbeVSurface::changeStateProbing);
    connect(ui->pushButtonOpenVideo, &QPushButton::clicked, this, &MainWindow::openVideo);
    connect(ui->pushButtonPlay, &QPushButton::clicked, this, &MainWindow::play);
    connect(ui->pushButtonOpenImage, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(ui->horizontalSliderSeek, &QSlider::sliderMoved, this, &MainWindow::setVideoPos);

    connect(ui->pushButtonCamera, &QPushButton::clicked, this, &MainWindow::setCamera);
    ui->pushButtonCamera->setEnabled( QCameraInfo::availableCameras().count() > 0 );

    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::mediaStateChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);

    ui->graphicsView->viewport()->installEventFilter(this);

    //TODO: remove then this temp and debug code
    cout << "main thread " << QThread::currentThreadId() << endl;
    ui->pushButtonOpenVideo->setEnabled(false); // ...as this doesn't work
    ui->pushButtonOpenDetector->setEnabled(false); // ...as this is not implemented yet

    proc->moveToThread(&procThread);
    connect(probe, &FrameProbeVSurface::frameProbed, proc, &Processor::receiveFrame, Qt::QueuedConnection);
    procThread.start();
}

MainWindow::~MainWindow()
{
    procThread.quit();
    procThread.wait();
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
    Q_UNUSED(event)

    ui->graphicsView->fitInView(vitem, Qt::KeepAspectRatio);
}

void MainWindow::openVideo() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Video"), QDir::homePath(), tr("Video Files (*)"));

    if (!fileName.isEmpty()) {
        clear();

        player->setVideoOutput(probe);
        player->setMedia(QUrl::fromLocalFile(fileName));
        player->play();
        ui->pushButtonPlay->setEnabled(true);

//        vitem->setSize(QSizeF(ui->graphicsView->rect().size()));
        ui->graphicsView->fitInView(vitem, Qt::KeepAspectRatio);
    }
}

void MainWindow::setCamera() {
    // It is supposed that this method invoked only when there exist cameras.
    clear();
    auto cameras = QCameraInfo::availableCameras();

    if (cameras.count() == 1) {
        camera = new QCamera(cameras.at(0));
    } else {
        //TODO: dialog to choose camera
        camera = new QCamera();
    }

    camera->setViewfinder(probe);
    camera->start();

//    vitem->setSize(QSizeF(ui->graphicsView->rect().size()));
    ui->graphicsView->fitInView(vitem, Qt::KeepAspectRatio);
}

void MainWindow::openImage() {
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QDir::homePath(), tr("Image Files (*)"));

    if (!filename.isEmpty()) {
        clear();
        probe->presentImage(QImage(filename));

        ui->graphicsView->fitInView(vitem, Qt::KeepAspectRatio);
    }
}

void MainWindow::clear() {
    if (camera) {
        camera->stop();
        delete camera;
        camera = nullptr;
    }

    player->setMedia(QMediaContent());
    ui->pushButtonPlay->setEnabled(false);
}

void MainWindow::play() {
    switch(player->state()) {
    case QMediaPlayer::PlayingState:
        player->pause();
        break;
    default:
        player->play();
        break;
    }
}

void MainWindow::setVideoPos(int pos) {
    player->setPosition(pos);
}

void MainWindow::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void MainWindow::positionChanged(qint64 position)
{
    ui->horizontalSliderSeek->setValue(position);
}

void MainWindow::durationChanged(qint64 duration)
{
    ui->horizontalSliderSeek->setRange(0, duration);
}
