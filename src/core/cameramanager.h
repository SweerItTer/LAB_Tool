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

    int CheckCameras(QList<QCameraInfo> &camerasInfo);

signals:
    void originalFrameProcessed(const QImage &frame);
    void binaryFrameProcessed(const QImage &frame);

private:
    void processFrames();

    cv::VideoCapture m_capture; // 摄像头对象
    QThread m_processingThread; // 线程对象
};

#endif // CAMERAMANAGER_H