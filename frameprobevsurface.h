#pragma once

#include <QAbstractVideoSurface>
#include <QGraphicsPixmapItem>
#include <QVideoFrame>

class FrameProbeVSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    FrameProbeVSurface(QGraphicsPixmapItem *display, QObject *parent = nullptr);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const override;
    bool present(const QVideoFrame &frame) override;

signals:
//    void frameProbed(const QVideoFrame &frame);
    void frameProbed(QImage frame);

public slots:
    void startProbing();
    void stopProbing();

private:
    bool isProbing_;
    QList<QVideoFrame::PixelFormat> supportedPixelFormats_;
    QGraphicsPixmapItem *img_item_;

};
