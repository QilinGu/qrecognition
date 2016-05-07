#include "frameprobevsurface.h"

#include <iostream>
#include <chrono>

#include <QThread>
#include <QImage>

#include "util.h"

FrameProbeVSurface::FrameProbeVSurface(QGraphicsPixmapItem *display, QObject *parent)
    : QAbstractVideoSurface(parent)
    , isProbing_(false)
    , isPaused_(false)
    , supportedPixelFormats_()
    , img_item_(display)
{
    supportedPixelFormats_
        << QVideoFrame::Format_ARGB32
        << QVideoFrame::Format_ARGB32_Premultiplied
        << QVideoFrame::Format_RGB32
        << QVideoFrame::Format_RGB24
        << QVideoFrame::Format_RGB565
        << QVideoFrame::Format_RGB555
        << QVideoFrame::Format_ARGB8565_Premultiplied
        << QVideoFrame::Format_BGRA32
        << QVideoFrame::Format_BGRA32_Premultiplied
        << QVideoFrame::Format_BGR32
        << QVideoFrame::Format_BGR24
        << QVideoFrame::Format_BGR565
        << QVideoFrame::Format_BGR555
        << QVideoFrame::Format_BGRA5658_Premultiplied
        << QVideoFrame::Format_AYUV444
        << QVideoFrame::Format_AYUV444_Premultiplied
        << QVideoFrame::Format_YUV444
        << QVideoFrame::Format_YUV420P
        << QVideoFrame::Format_YV12
        << QVideoFrame::Format_UYVY
        << QVideoFrame::Format_YUYV
        << QVideoFrame::Format_NV12
        << QVideoFrame::Format_NV21
        << QVideoFrame::Format_IMC1
        << QVideoFrame::Format_IMC2
        << QVideoFrame::Format_IMC3
        << QVideoFrame::Format_IMC4
        << QVideoFrame::Format_Y8
        << QVideoFrame::Format_Y16
        << QVideoFrame::Format_Jpeg
        << QVideoFrame::Format_CameraRaw
        << QVideoFrame::Format_AdobeDng;
}


QList<QVideoFrame::PixelFormat> FrameProbeVSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const {
    Q_UNUSED(handleType);

    return supportedPixelFormats_;
}

bool FrameProbeVSurface::present(const QVideoFrame &frame)
{
    if (frame.isValid()) {

        QImage img = Util::convertToImage(frame);
        if (!img.isNull()) {
            img_item_->setPixmap(QPixmap::fromImage(img));

            ++i;
            if (isProbing_ && !isPaused_) {

                auto now = std::chrono::high_resolution_clock::now();
                auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(now);
                auto val = now_ms.time_since_epoch();
                long dur = val.count();

                emit frameProbed(img); // latency between sending and receiving is about 60 microseconds
                std::cout << "frame " << i << " presented at " << dur << std::endl;
            }
            return true;
        }
    }
    return false;
}

bool FrameProbeVSurface::presentImage(QImage img) {
    if (!img.isNull()) {
        img_item_->setPixmap(QPixmap::fromImage(img));
        emit frameProbed(img);
        return true;
    }
    return false;
}

bool FrameProbeVSurface::isProbing() {
    return isProbing_;
}

bool FrameProbeVSurface::start(const QVideoSurfaceFormat &format) {
    return QAbstractVideoSurface::start(format);
}

void FrameProbeVSurface::continueProbing() {
    isPaused_ = false;
}

void FrameProbeVSurface::pauseProbing() {
    isPaused_ = true;
}

void FrameProbeVSurface::changeStateProbing() {
    isProbing_ = !isProbing_;
}
