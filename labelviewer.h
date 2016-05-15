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
    void resizeEvent(QResizeEvent *event = nullptr) override;

private:
    QLabel *bg_display_; // no ownership
    QLabel *fg_display_; // no ownership (bg_display is parent)
    QPixmap image_;
    QPixmap overlay_;
};
