#ifndef FRAMEIMAGEPROVIDER_H
#define FRAMEIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

class FrameImageProvider : public QQuickImageProvider {
public:
    FrameImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        Q_UNUSED(id);
        Q_UNUSED(requestedSize);
        
        QMutexLocker locker(&mutex);
        if (size)
            *size = frame.size();
        return frame;
    }

    void setImage(const QImage &image) {
        QMutexLocker locker(&mutex);
        frame = image;
        qDebug() << "setImage";
    }

private:
    QImage frame;
    QMutex mutex;
};

extern FrameImageProvider *imageProvider_raw;
extern FrameImageProvider *imageProvider_binary;

#endif // FRAMEIMAGEPROVIDER_H
