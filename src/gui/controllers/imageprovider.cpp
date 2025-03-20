#include "imageprovider.h"
#include <QDebug>

FrameImageProvider::FrameImageProvider(QObject *parent)
    : QObject(parent),
      QQuickImageProvider(QQuickImageProvider::Image),
      m_mutex(QMutex::NonRecursive)  // 初始化mutex
{
}

QImage FrameImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    QMutexLocker locker(&m_mutex);
    if (size) *size = m_image.size();
    return requestedSize.isValid() ? m_image.scaled(requestedSize, Qt::KeepAspectRatio) : m_image;
}

void FrameImageProvider::setImage(const QImage& image) {
    // qDebug() << "GetImage";
    QMutexLocker locker(&m_mutex);
    m_image = image;
    emit imageChanged();
}
