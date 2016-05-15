#pragma once

#include <QMainWindow>
#include <QThread>
#include <QString>

#include <QGraphicsScene>

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

    void positionChanged(qint64 pos);
    void durationChanged(qint64 dur);
    void playbackSpeedChanged(qint64 fps);

private slots:
    void resizeEvent(QResizeEvent *event) override;

private:
    void reset();

    QThread thread1;
    QThread thread2;
    QThread thread3;
    QThread thread4;

    Capture *capture;
    Converter *converter;
    AbstractViewer *viewer;
    AbstractOutput *output;
    Processor *proc;

    Ui::MainWindow *ui;

    std::vector<std::string> supported_img_formats_;
    std::vector<std::string> supported_video_formats_;

    void initSupportedFormats();
    QString formatsToPatterns(const std::vector<std::string> &formats);
};
