#pragma once

#include <QObject>
#include <QImage>
#include <QLabel>

#include "abstractviewer.h"

class LabelViewer : public AbstractViewer
{
public:
    LabelViewer(QLabel *display, QObject *parent = nullptr);

    void displayImage(const QImage &img) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *display_;

    /*dbg*/int i = 0;
};
