#pragma once

#include <QMainWindow>
#include <QThread>
#include <QString>

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
    void openVideo();
    void openImage();
    void openCamera();

    void positionChanged(qint64 pos);
    void durationChanged(qint64 dur);
    void playbackSpeedChanged(qint64 fps);

private slots:
    void resizeEvent(QResizeEvent *event) override;

private:
    QThread thread1_;
    QThread thread2_;
    QThread thread3_;
    QThread thread4_;

    Capture *capture_; // ownership
    Converter *converter_; // ownership
    AbstractViewer *viewer_; // ownership
    AbstractOutput *output_; // ownership
    Processor *proc_; // ownership

    std::vector<std::string> supported_img_formats_;
    std::vector<std::string> supported_video_formats_;

    Ui::MainWindow *ui;

    void reset();

    void initSupportedFormats();
    QString formatsToPatterns(const std::vector<std::string> &formats);
};
