#pragma once

#include <memory>

#include <QThread>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoProbe>
#include <QGraphicsView>
#include <QGraphicsVideoItem>

#include "opennetdialog.h"
#include "frameprober.h"
#include "processor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void play();
    void openVideo();
    void openImage();
    void setVideoPos(int pos);

    void openClassifier();
    void openDetector();

    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 pos);
    void durationChanged(qint64 dur);

private slots:
    void startProcessing();

private:
    QThread procThread;

    QMediaPlayer *player;
    QGraphicsVideoItem *vitem;
    FrameProbe *probe;
    QGraphicsPixmapItem *img_item;

    Ui::MainWindow *ui;
    OpenNetDialog *net_dialog;
    Processor *proc;

    bool isProcessing;


    void clear();
};
