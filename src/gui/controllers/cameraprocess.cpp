#include "cameraprocess.h"

#include "imageprovider.h"
#include "src/core/cameramanager.h"

CameraProcess::CameraProcess(QObject *parent) : QObject(parent) {
    m_cameraManager = new CameraManager();
    // 连接 OpenCV 处理结果到 Qt 信号
    connect(m_cameraManager, &CameraManager::originalFrameProcessed,
        imageProvider_raw, &FrameImageProvider::setImage);
    connect(m_cameraManager, &CameraManager::binaryFrameProcessed,
        imageProvider_binary, &FrameImageProvider::setImage);
}

CameraProcess::~CameraProcess() {
    delete m_cameraManager;
    // 交由main管理
    // delete imageProvider_raw;
    // delete imageProvider_binary;
}

void CameraProcess::startProcess(int index)
{
    m_cameraManager->startProcessing(index); // 启动 OpenCV 处理线程
}

void CameraProcess::stopProcess()
{
    m_cameraManager->stopProcessing(); // 启动 OpenCV 处理线程
}

void CameraProcess::check()
{
    QStringList cameraNames;
    QList<QCameraInfo> camerasInfo;
    // 获取摄像头信息
    int size = m_cameraManager->CheckCameras(camerasInfo);
    if (size == -1) {
        // 报错(无摄像头)
        emit checkError();
        return;
    }
    for (const auto &camera : camerasInfo) {
        // 转换为 QString
        cameraNames.append(camera.description());
    }
    // 返回摄像头信息和标志
    emit checkWarning(cameraNames, size > 1 ? 1 : 0);
    // qDebug() << "check\nCameraNames:" << cameraNames << "\nCameraSize:" << camerasInfo.size();
}
