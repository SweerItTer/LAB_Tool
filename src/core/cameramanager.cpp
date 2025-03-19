#include <iostream>
#include <thread>
#include <chrono>

#include <QDebug>

#include "cameramanager.h"

using namespace std;

CameraManager::CameraManager(QObject *parent) : QObject(parent) {
}

CameraManager::~CameraManager()
{
    // 停止子线程
    stopProcessing();
}

void CameraManager::startProcessing(int index)
{
    m_capture.open(index); // 打开摄像头
    if(!m_capture.isOpened()) {
        qWarning() << "无法打开摄像头";
        return;
    }
    std::thread(&CameraManager::processFrames, this).detach();
}

void CameraManager::stopProcessing()
{
    m_exitFlag.store(true);
    qDebug() << "CameraManager::stopProcessing()";
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
    // 原图 二值图
    cv::Mat frame, binaryFrame;
    while (m_exitFlag.load() != true) {
        // 取帧
        m_capture >> frame;
        if (frame.empty()) {
            qWarning() << "无法读取摄像头帧";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        };

        // 转换为灰度图并二值化
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // 将BGR转换为RGB
        cv::cvtColor(frame, binaryFrame, cv::COLOR_RGB2GRAY);// 灰度图
        cv::threshold(binaryFrame, binaryFrame, 128, 255, cv::THRESH_BINARY);// 二值化

        // 转换为 QImage 并发送信号
        /*-------- 优化方向:OpenGL 直接渲染 ------ */
        // 修复从 size_t 到 int 的转换可能丢失数据的问题
        QImage originalQImg(frame.data,
            static_cast<int>(frame.cols),
            static_cast<int>(frame.rows),
            static_cast<int>(frame.step),
            QImage::Format_RGB888);
        QImage binaryQImg(binaryFrame.data, 
            static_cast<int>(binaryFrame.cols), 
            static_cast<int>(binaryFrame.rows), 
            static_cast<int>(binaryFrame.step), 
            QImage::Format_Grayscale8);
        // 传递原始帧和二值化帧到 QML
        emit originalFrameProcessed(originalQImg.copy());
        emit binaryFrameProcessed(binaryQImg.copy());
        // 控制帧率 30
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    m_capture.release(); // 释放摄像头
}