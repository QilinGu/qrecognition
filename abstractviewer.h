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
    /* Output image.
     * @param img image to be outputted. */
    virtual void displayImage(const QImage &img) = 0;

    /* Set overlay.
     * @param pm overlay to be outputted. */
    virtual void setOverlay(const QPixmap &pm) = 0;

    virtual void resizeEvent(QResizeEvent *event) = 0;
};
