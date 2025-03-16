#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "gui/controllers/screenshotprovider.h" 
#include "gui/controllers/dataprocess.h"
#include "gui/controllers/cameraprocess.h"
#include "gui/controllers/imageprovider.h"

FrameImageProvider *imageProvider_raw = nullptr;
FrameImageProvider *imageProvider_binary = nullptr;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;

	// 创建并注册对象(主程序管理)
	ScreenshotProvider *screenshotProvider = new ScreenshotProvider();
    imageProvider_raw = new FrameImageProvider();
    imageProvider_binary = new FrameImageProvider();
	// 图像提供器注册
    engine.addImageProvider("frameprovider_raw", imageProvider_raw);
    engine.addImageProvider("frameprovider_binary", imageProvider_binary);

	engine.addImageProvider("screenshotProvider", screenshotProvider);

	engine.rootContext()->setContextProperty("screenshotProvider", screenshotProvider);
	
	// 注册对象(控制器)
	qmlRegisterType<DataProcess>("gui.controllers", 1, 0, "DataProcess");
	qmlRegisterType<CameraProcess>("gui.controllers", 1, 0, "CameraProcess");
	
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
