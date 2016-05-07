#pragma once

#include <QAbstractVideoSurface>
#include <QGraphicsPixmapItem>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>

class FrameProbeVSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    FrameProbeVSurface(QGraphicsPixmapItem *display, QObject *parent = nullptr);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const override;
    bool present(const QVideoFrame &frame) override;
    bool start(const QVideoSurfaceFormat &format) override;

    bool presentImage(QImage img);

    bool isProbing();

signals:
    void frameProbed(QImage frame);

public slots:
    void continueProbing();
    void pauseProbing();
    void changeStateProbing();

private:
    bool isProbing_;
    bool isPaused_;
    QList<QVideoFrame::PixelFormat> supportedPixelFormats_;
    QGraphicsPixmapItem *img_item_;

    int i = 0;

};
