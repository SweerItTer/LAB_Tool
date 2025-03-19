#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>
#include <QThread>
#include <QImage>
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

signals:
    void originalFrameProcessed(const QImage &frame);
    void binaryFrameProcessed(const QImage &frame);

private:
    void processFrames();

    // 修复可能由于初始化语法引起的问题，确保类型和值匹配
    std::atomic<bool> m_exitFlag{false};
    cv::VideoCapture m_capture; // 摄像头对象
};

#endif // CAMERAMANAGER_H