#pragma once

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QResizeEvent>

class AbstractViewer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractViewer(QObject *parent = 0) : QObject(parent) {}
    virtual ~AbstractViewer() {}

signals:
    void fg_ready(const QPixmap &fg);
    void bg_ready(const QPixmap &bg);

public slots:
    virtual void displayImage(const QImage &img) = 0;
    virtual void setOverlay(const QPixmap &pm) = 0;
    virtual void resizeEvent(QResizeEvent *event) = 0;
};
