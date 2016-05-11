#pragma once

#include <QMainWindow>
#include <QThread>

#include <QGraphicsScene>

#include "opennetdialog.h"
#include "abstractoutput.h"
#include "abstractviewer.h"
#include "processor.h"
#include "capture.h"
#include "converter.h"

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
    void setVideo();
    void setImage();
    void setCamera();

    void play();
    void setVideoPos(int pos);
//    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 pos);
    void durationChanged(qint64 dur);

private slots:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    void clear();

    QThread thread1;
    QThread thread2;
    QThread thread3;
    QThread thread4;

    QGraphicsScene *scene;

    Capture *capture;
    Converter *converter;
    AbstractViewer *viewer;
    AbstractOutput *output;
    Processor *proc;

    OpenNetDialog *net_dialog;
    Ui::MainWindow *ui;
};
