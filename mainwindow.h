#pragma once

#include <memory>

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoProbe>
#include <QGraphicsView>
#include <QGraphicsVideoItem>

#include "inputcenter.h"

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

    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 pos);
    void durationChanged(qint64 dur);

private:
    QMediaPlayer *player; // ownership
    QGraphicsVideoItem *vitem; // ownership
    QVideoProbe *probe;

    Ui::MainWindow *ui;
    InputCenter *in;
};
