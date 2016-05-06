#pragma once

#include <memory>

#include <QThread>
#include <QMainWindow>

#include <QMediaPlayer>
#include <QVideoProbe>
#include <QGraphicsView>
#include <QGraphicsVideoItem>

#include <QCamera>

#include "opennetdialog.h"
#include "frameprobevsurface.h"
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
    void setCamera();
    void setVideoPos(int pos);

    void openClassifier();
    void openDetector();

    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 pos);
    void durationChanged(qint64 dur);

private slots:
    void changeStateProbing();
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    void clear();

    QThread procThread;
    QThread probThread;

    QGraphicsScene *scene;
    QMediaPlayer *player;
    FrameProbeVSurface *probe;
    QGraphicsPixmapItem *vitem;
    QCamera *camera;

    Ui::MainWindow *ui;
    OpenNetDialog *net_dialog;
    Processor *proc;
};
