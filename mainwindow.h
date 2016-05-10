#pragma once

#include <QMainWindow>
#include <QThread>

#include <QCamera>
#include <QMediaPlayer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "opennetdialog.h"
#include "frameprobevsurface.h"
#include "processor.h"
#include "abstractoutput.h"

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
    void openVideo();
    void openImage();
    void setCamera();

    void play();
    void setVideoPos(int pos);
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 pos);
    void durationChanged(qint64 dur);

private slots:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    void clear();

    QThread procThread;
    QThread probThread;

    QGraphicsScene *scene;
    QMediaPlayer *player;
    QCamera *camera;

    QGraphicsPixmapItem *vitem;
    FrameProbeVSurface *probe;
    AbstractOutput *output;

    Ui::MainWindow *ui;
    OpenNetDialog *net_dialog;
    Processor *proc;
};
