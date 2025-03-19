#ifndef SCREENSHOTPROVIDER_H
#define SCREENSHOTPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QGuiApplication>
#include <QPixmap>
#include <QImage>
#include <QScreen>
#include <QDebug>

// 全屏截图 + 获取像素颜色
class ScreenshotProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit ScreenshotProvider(QObject *parent = nullptr);  // 添加构造函数声明

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    Q_INVOKABLE void captureScreenshot();
    Q_INVOKABLE QColor getPixelColor(int x, int y);
    Q_INVOKABLE QColor getColor() const;

signals:
    void screenshotUpdated();

private:
    QColor qColor;
    QPixmap screenshot_P;
    QImage screenshot;
};

#endif // SCREENSHOTPROVIDER_H