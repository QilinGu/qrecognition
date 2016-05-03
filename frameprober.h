#pragma once

#include <QObject>
#include <QVideoProbe>

class FrameProbe : public QObject
{
    Q_OBJECT

public:
    explicit FrameProbe(QObject *parent = 0);

    bool setSource(QMediaObject *source);
    bool isActive() const;

public slots:
    void stopProbing();
    void continueProbing();

signals:
    void videoFrameProbed(const QVideoFrame &frame);

private slots:
    void wrapVideoFrameProbedSignal(const QVideoFrame &frame);

private:
    QVideoProbe probe_;
    QMediaObject *last_;
};
