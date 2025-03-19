// ScreenshotProvider.cpp
#include "screenshotprovider.h"

ScreenshotProvider::ScreenshotProvider(QObject *parent)
    : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image)
{
    // 显式初始化成员变量
    qColor = QColor();
    screenshot_P = QPixmap();
    screenshot = QImage();
}

QImage ScreenshotProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    Q_UNUSED(id);
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);
    return screenshot;
}

void ScreenshotProvider::captureScreenshot() {
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        qDebug() << "Screen Geometry (Logical):" << screenGeometry;

        screenshot_P = screen->grabWindow(0, screenGeometry.x(), screenGeometry.y(),
                                        screenGeometry.width(), screenGeometry.height());
        screenshot_P.setDevicePixelRatio(1);
        qDebug() << "Screenshot Size (Physical):" << screenshot_P.size();
        screenshot = screenshot_P.toImage();
        emit screenshotUpdated();
    }
}

QColor ScreenshotProvider::getPixelColor(int x, int y) {
    if (screenshot.isNull() || x < 0 || x >= screenshot.width() || y < 0 || y >= screenshot.height()) {
        return QColor();
    }
    return screenshot.pixelColor(x, y);
}

QColor ScreenshotProvider::getColor() const {
    return qColor;
}