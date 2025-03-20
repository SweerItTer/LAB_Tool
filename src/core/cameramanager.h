#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QString>
#include <QMutex>
#include <QList>
#include <QCameraInfo>

#include <opencv2/opencv.hpp>

class CameraManager : public QObject {
    Q_OBJECT
public:
    explicit CameraManager(QObject *parent = nullptr);
    ~CameraManager();

    void startProcessing(int index);
    void stopProcessing();
    int CheckCameras(QList<QCameraInfo> &camerasInfo);
    void setValue(const QString &id, double min, double max);
signals:
    void originalFrameProcessed(const QImage &frame);
    void binaryFrameProcessed(const QImage &frame);

private:
    void processFrames();
    cv::VideoCapture m_capture; // 摄像头对象
    // 修复可能由于初始化语法引起的问题，确保类型和值匹配
    std::atomic<bool> m_exitFlag{false};
    double lMin = 0.0, lMax = 255.0;
    double aMin = 0.0, aMax = 255.0;
    double bMin = 0.0, bMax = 255.0;

    QMutex m_mutex;

};

#endif // CAMERAMANAGER_H