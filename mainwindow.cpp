#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QUrl>
#include <QFileDialog>
#include <QDir>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , player(new QMediaPlayer())
    , vitem(new QGraphicsVideoItem)
    , probe(new QVideoProbe)
    , ui(new Ui::MainWindow)
    , in(new InputCenter(this))
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

    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::mediaStateChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);

    // TODO: set a checking
    probe->setSource(player);
    connect(probe, &QVideoProbe::videoFrameProbed, in, &InputCenter::testProcessProbe);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openVideo() {
    ui->graphicsView->scene()->clear();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());

    if (!fileName.isEmpty()) {
        player->setMedia(QUrl::fromLocalFile(fileName));
        ui->pushButtonPlay->setEnabled(true);
    }
}

void MainWindow::openImage() {
    ui->graphicsView->scene()->clear();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());

    if (!fileName.isEmpty()) {
        QImage img(fileName);

        if(!img.isNull()) {
            QPixmap pmap(fileName);
            auto item = new QGraphicsPixmapItem(pmap);
            ui->graphicsView->scene()->addItem(item);
            ui->graphicsView->show();
        }
    }
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
