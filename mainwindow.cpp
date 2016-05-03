#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QUrl>
#include <QFileDialog>
#include <QDir>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , probThread()
    , procThread()
    , player(new QMediaPlayer())
    , vitem(new QGraphicsVideoItem)
    , probe(new FrameProbe())
    , img_item(nullptr)
    , ui(new Ui::MainWindow)
    , proc(new Processor(probe, &probThread))
    , isProcessing(false)
{
    ui->setupUi(this);

    auto scene = new QGraphicsScene(this);
    ui->graphicsView->setGeometry(0, 0, 800, 600);
    scene->setSceneRect(ui->graphicsView->rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

    vitem->setPos(0, 0);
    vitem->setSize(QSizeF(ui->graphicsView->size()));
    scene->addItem(vitem);
    player->setVideoOutput(vitem);

    ui->pushButtonPlay->setEnabled(false);
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->horizontalSliderSeek->setRange(0, 0);

    connect(ui->pushButtonOpenVideo, &QPushButton::clicked, this, &MainWindow::openVideo);
    connect(ui->pushButtonPlay, &QPushButton::clicked, this, &MainWindow::play);
    connect(ui->pushButtonOpenImage, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(ui->horizontalSliderSeek, &QSlider::sliderMoved, this, &MainWindow::setVideoPos);
    connect(ui->pushButtonStartCl, &QPushButton::clicked, this, &MainWindow::startProcessing);

    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::mediaStateChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);

    cout << "main thread " << QThread::currentThreadId() << endl;

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

void MainWindow::openVideo() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());

    if (!fileName.isEmpty()) {
        clear();
        player->setMedia(QUrl::fromLocalFile(fileName));
        ui->pushButtonPlay->setEnabled(true);
    }
}

void MainWindow::openImage() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());

    if (!fileName.isEmpty()) {
        clear();
        QImage img(fileName);

        if(!img.isNull()) {
            QPixmap pmap(fileName);
            img_item = new QGraphicsPixmapItem(pmap);
            ui->graphicsView->scene()->addItem(img_item);
            ui->graphicsView->show();
        }
    }
}

void MainWindow::clear() {
    if (img_item) {
        ui->graphicsView->scene()->removeItem(img_item);
    }
    player->setMedia(QMediaContent());
    probe->stopProbing();
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

//TODO: remove starting / pausing to Processor
void MainWindow::startProcessing()
{
    if (isProcessing) {
        probe->stopProbing();
    } else if (!probe->setSource(player)) {
        qDebug() << "Current QMediaObject doesn't support monitoring video";
    }
}
