#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

class FrameImageProvider : public QObject, public QQuickImageProvider {
    Q_OBJECT
public:
    explicit FrameImageProvider(QObject *parent = nullptr);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

public slots:
    void setImage(const QImage& image);

signals:
    void imageChanged();

private:
    QImage m_image;
    QMutex m_mutex;
};

// 全局变量声明
extern FrameImageProvider *imageProvider_raw;
extern FrameImageProvider *imageProvider_binary;

#endif // IMAGEPROVIDER_H