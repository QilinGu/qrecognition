#pragma once

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include "abstractviewer.h"

class LabelViewer : public AbstractViewer
{
public:
    LabelViewer(QLabel *display, QObject *parent = nullptr);

    void displayImage(const QImage &img) override;
    void setOverlay(const QPixmap &pm) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *bg_display_;
    QLabel *fg_display_;
    QPixmap image_;
    QPixmap overlay_;
};
