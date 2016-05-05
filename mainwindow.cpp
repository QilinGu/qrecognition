#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QUrl>
#include <QFileDialog>
#include <QDir>
#include <QResizeEvent>
#include <QCameraInfo>

#include "opennetdialog.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , procThread()
    , player(new QMediaPlayer())
    , vitem(new QGraphicsVideoItem)
    , probe(new FrameProbe())
    , img_item(nullptr)
    , camera(nullptr)
    , ui(new Ui::MainWindow)
    , net_dialog(new OpenNetDialog(this))
    , proc(new Processor(probe))
    , isProcessing(false)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
//    ui->graphicsView->setGeometry(0, 0, 592, 478);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

//    vitem->setPos(0, 0);
    scene->addItem(vitem);
    ui->graphicsView->fitInView(vitem);
    player->setVideoOutput(vitem);

    ui->pushButtonPlay->setEnabled(false);
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->horizontalSliderSeek->setRange(0, 0);

    connect(ui->pushButtonOpenClassifier, &QPushButton::clicked, this, &MainWindow::openClassifier);
    connect(ui->pushButtonOpenDetector, &QPushButton::clicked, this, &MainWindow::openDetector);
    connect(ui->pushButtonOpenVideo, &QPushButton::clicked, this, &MainWindow::openVideo);
    connect(ui->pushButtonPlay, &QPushButton::clicked, this, &MainWindow::play);
    connect(ui->pushButtonOpenImage, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(ui->horizontalSliderSeek, &QSlider::sliderMoved, this, &MainWindow::setVideoPos);
    connect(ui->pushButtonStartCl, &QPushButton::clicked, this, &MainWindow::startProcessing);

    connect(ui->pushButtonCamera, &QPushButton::clicked, this, &MainWindow::setCamera);
    ui->pushButtonCamera->setEnabled( QCameraInfo::availableCameras().count() > 0 );

    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::mediaStateChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);

    ui->graphicsView->viewport()->installEventFilter(this);

//    cout << "main thread " << QThread::currentThreadId() << endl;

    //TODO: maybe this is Processor logic?
    probe->moveToThread(&procThread);
    proc->moveToThread(&procThread);
    connect(probe, &FrameProbe::videoFrameProbed, proc, &Processor::receiveFrame, Qt::DirectConnection);
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

    if (img_item) {
        ui->graphicsView->fitInView(img_item, Qt::KeepAspectRatio);
    } else {
        ui->graphicsView->fitInView(vitem, Qt::KeepAspectRatio);
    }
}

//TODO: make user know that it is not okey when net is not loaded
void MainWindow::openClassifier() {
    net_dialog->exec();

    if (net_dialog->result() == QDialog::Accepted && net_dialog->isFilesChoosed()) {
        proc->initClassifier(net_dialog->filePaths());
    }
}

//TODO: make user know that it is not okey when net is not loaded
void MainWindow::openDetector() {
    net_dialog->exec();

    if (net_dialog->result() == QDialog::Accepted && net_dialog->isFilesChoosed()) {
        proc->initDetector(net_dialog->filePaths());
    }
}

void MainWindow::openVideo() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Video"), QDir::homePath(), tr("Video Files (*)"));

    if (!fileName.isEmpty()) {
        clear();
        player->setMedia(QUrl::fromLocalFile(fileName));
        ui->pushButtonPlay->setEnabled(true);

        vitem->setSize(QSizeF(ui->graphicsView->rect().size()));
    }
}

void MainWindow::openImage() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QDir::homePath(), tr("Image Files (*)"));

    if (!fileName.isEmpty()) {
        clear();
        QImage img(fileName);

        if(!img.isNull()) {
            img_item = new QGraphicsPixmapItem(QPixmap::fromImage(img));
            scene->setSceneRect(QRectF()); //reinit scene for normal image resizing
            scene->addItem(img_item);
            ui->graphicsView->show();

            ui->graphicsView->fitInView(img_item, Qt::KeepAspectRatio);
        }
    }
}

void MainWindow::setCamera() {
    // it is supposed that this method invoked only when there exist cameras
    clear();
    auto cameras = QCameraInfo::availableCameras();

    if (cameras.count() == 1) {
        camera = new QCamera(cameras.at(0));
    } else {
        //TODO:
        //dialog to choose camera
        camera = new QCamera();
    }

    vitem->setSize(QSizeF(ui->graphicsView->rect().size()));
//    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->setViewfinder(vitem);
    camera->start();
}

void MainWindow::clear() {
    if (img_item) {
        ui->graphicsView->scene()->removeItem(img_item);
        delete img_item;
        img_item = nullptr;
    }

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

//TODO: move starting / pausing to Processor
void MainWindow::startProcessing()
{
    //refactor to proc:
    //emit changeStateProcessing(QMediaObject *obj);

    if (isProcessing) {
        isProcessing = false;
        probe->stopProbing();
    } else if (camera) {
        if (!probe->setSource(camera))
            qDebug() << "Current QMediaObject doesn't support monitoring video";
    } else if (!probe->setSource(player)) {
        qDebug() << "Current QMediaObject doesn't support monitoring video";
    }
}
