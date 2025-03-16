#include <iostream>
#include <thread>
#include <chrono>

#include <QDebug>

#include "cameramanager.h"

using namespace std;

CameraManager::CameraManager(QObject *parent) : QObject(parent) {
    moveToThread(&m_processingThread); // 将线程对象移至子线程
    m_processingThread.start(); // 启动子线程
}

CameraManager::~CameraManager()
{
    // 停止子线程
    m_processingThread.quit();
    m_processingThread.wait();
}

void CameraManager::startProcessing(int index) {
    m_capture.open(index); // 打开默认摄像头
    if (!m_capture.isOpened()) {
        qDebug() << QStringLiteral("无法打开摄像头");
        return;
    }
    processFrames();
}

int CameraManager::CheckCameras(QList<QCameraInfo> &camerasInfo) {
    // 获取所有可用摄像头
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    if (cameras.isEmpty()) {
        std::cout << "No available cameras found!" << std::endl;
        return -1;
    }
    camerasInfo = cameras;
    return cameras.size();
}

void CameraManager::processFrames() {
    cv::Mat frame, binaryFrame;
    while (true) {
        // 控制帧率
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        // 取帧
        m_capture >> frame;
        if (frame.empty()) break;

        // 转换为灰度图并二值化
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        cv::cvtColor(frame, binaryFrame, cv::COLOR_RGB2GRAY);
        cv::threshold(binaryFrame, binaryFrame, 128, 255, cv::THRESH_BINARY);

        // 转换为 QImage 并发送信号
        /*-------- 优化方向:OpenGL 直接渲染 ------ */
        QImage originalQImg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        QImage binaryQImg(binaryFrame.data, binaryFrame.cols, binaryFrame.rows, binaryFrame.step, QImage::Format_Grayscale8);
        // 传递原始帧和二值化帧到 QML
        emit originalFrameProcessed(originalQImg.copy());
        emit binaryFrameProcessed(binaryQImg.copy());
    }
}