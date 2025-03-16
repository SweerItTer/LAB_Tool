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
class ScreenshotProvider : public QObject, public QQuickImageProvider {
	Q_OBJECT
public:
	ScreenshotProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

	QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
		Q_UNUSED(id)
		Q_UNUSED(size)
		Q_UNUSED(requestedSize)
		return screenshot;
	}

	Q_INVOKABLE void captureScreenshot() {
	   QScreen *screen = QGuiApplication::primaryScreen();
	   if (screen) {
			// 获取屏幕的几何信息（逻辑分辨率，如1440x960）
			QRect screenGeometry = screen->geometry();
			qDebug() << "Screen Geometry (Logical):" << screenGeometry;

			// 截取整个屏幕的物理区域
			screenshot_P = screen->grabWindow(0,
											screenGeometry.x(),
											screenGeometry.y(),
											screenGeometry.width(),
											screenGeometry.height());

			// 设置截图的设备像素比
			screenshot_P.setDevicePixelRatio(1);
			qDebug() << "Screenshot Size (Physical):" << screenshot_P.size();
			screenshot = screenshot_P.toImage();			
			emit screenshotUpdated();
	   }
   }
	Q_INVOKABLE QColor getPixelColor(int x, int y){
		if (screenshot.isNull()) {
			return QColor();
		}
		if (x >= 0 && x < screenshot.width() && y >= 0 && y < screenshot.height()) {
			qColor = screenshot.pixelColor(x, y);
			return qColor;
		}
		return QColor();
	}

	QColor getColor(){
		return qColor;
	}

signals:
	void screenshotUpdated();

private:
	QColor qColor;
	QPixmap screenshot_P;
	QImage screenshot;
};

#endif // SCREENSHOTPROVIDER_H
