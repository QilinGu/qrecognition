#include "frameprober.h"

FrameProbe::FrameProbe(QObject *parent)
    : QObject(parent)
    , probe_()
    , last_(nullptr)
{
    connect(&probe_, &QVideoProbe::videoFrameProbed, this, &FrameProbe::wrapVideoFrameProbedSignal);
}

void FrameProbe::wrapVideoFrameProbedSignal(const QVideoFrame &frame) {
    emit videoFrameProbed(frame);
}

bool FrameProbe::setSource(QMediaObject *source) {
    last_ = source;
    return probe_.setSource(source);
}

bool FrameProbe::isActive() const {
    return probe_.isActive();
}

void FrameProbe::stopProbing() {
    probe_.setSource(static_cast<QMediaObject *>(nullptr));
}

void FrameProbe::continueProbing() {
    probe_.setSource(last_);
}
