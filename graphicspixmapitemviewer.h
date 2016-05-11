#pragma once

#include <QObject>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "abstractviewer.h"

class GraphicsPixmapItemViewer : public AbstractViewer
{
public:
    GraphicsPixmapItemViewer(QGraphicsView *view, QGraphicsPixmapItem *display, QObject *parent = nullptr);

    void displayImage(const QImage &img) override;
    void setOverlay(const QPixmap &pm) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsView *view_;
    QGraphicsPixmapItem *display_;

    /*dbg*/int i = 0;
};
