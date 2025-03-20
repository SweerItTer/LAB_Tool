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

void CameraManager::setValue(const QString &id, double min, double max)
{
    QMutexLocker locker(&m_mutex);
    if(id == "L"){
        lMin = min;
        lMax = max;
    } else if (id == "A")
    {
        aMin = min;
        aMax = max;
    } else if (id == "B")
    {
        bMin = min;
        bMax = max;
    }
    qDebug() << id << min << max;
}

void CameraManager::processFrames() {
    cv::Mat frame, binaryFrame;// 原图 二值图
    std::vector<cv::Mat> labChannels;// lab通道
    // 独立通道
    cv::Mat* lChannel = nullptr;
    cv::Mat* aChannel = nullptr;
    cv::Mat* bChannel = nullptr;

    cv::Mat maskL, maskA, maskB;// 二值掩膜
    while (m_exitFlag.load() != true) {
        // 取帧
        m_capture >> frame;
        if (frame.empty()) {
            qWarning() << "无法读取摄像头帧";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        };

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // 将BGR转换为RGB
        cv::cvtColor(frame, binaryFrame, cv::COLOR_RGB2Lab); // 转换为LAB颜色空间
        
        cv::split(binaryFrame, labChannels);
        lChannel = &labChannels[0]; // L通道
        aChannel = &labChannels[1]; // A通道
        bChannel = &labChannels[2]; // B通道

        QMutexLocker locker(&m_mutex);
        // 将理论值转换为OpenCV的8位存储值
        // 0-255
        uchar cvLMin = static_cast<uchar>(lMin);
        uchar cvLMax = static_cast<uchar>(lMax);
        uchar cvAMin = static_cast<uchar>(aMin); 
        uchar cvAMax = static_cast<uchar>(aMax);
        uchar cvBMin = static_cast<uchar>(bMin);
        uchar cvBMax = static_cast<uchar>(bMax);
        locker.unlock();

        // 生成各通道的掩膜
        cv::inRange(*lChannel, cvLMin, cvLMax, maskL);  // L通道掩膜
        cv::inRange(*aChannel, cvAMin, cvAMax, maskA);  // A通道掩膜
        cv::inRange(*bChannel, cvBMin, cvBMax, maskB);  // B通道掩膜

        // 合并三个掩膜（逻辑与操作）
        cv::Mat finalMask;
        cv::bitwise_and(maskL, maskA, finalMask);
        cv::bitwise_and(finalMask, maskB, finalMask);

        // 输出结果
        finalMask.copyTo(binaryFrame);

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