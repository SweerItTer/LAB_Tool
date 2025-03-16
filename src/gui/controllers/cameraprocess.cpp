#include "cameraprocess.h"
#include "src/core/cameramanager.h"
#include "imageprovider.h"

CameraProcess::CameraProcess(QObject *parent) : QObject(parent) {
    m_cameraManager = new CameraManager();
    // 连接 OpenCV 处理结果到 Qt 信号
    // connect(m_cameraManager, &CameraManager::originalFrameProcessed,
    //         this, &CameraProcess::originalFrameReady);
    // connect(m_cameraManager, &CameraManager::binaryFrameProcessed,
    //         this, &CameraProcess::binaryFrameReady);
}

CameraProcess::~CameraProcess() {
    delete m_cameraManager;
}

void CameraProcess::startProcess(int index)
{

    // 问题:摄像头线程(??)启动主线程卡住(怀疑其实根本没有开启线程)
    // qDebug() << "start process\nCameraIndex:" << index;
    // m_cameraManager->startProcessing(index); // 启动 OpenCV 处理线程
}

void CameraProcess::originalFrameReady(const QImage &frame)
{
    qDebug() << "originalFrameReady";
    // 将原始帧传递给 QML
    // if(imageProvider_raw)
    //     imageProvider_raw->setImage(frame);
    // else
    //     qDebug() << "imageProvider_raw is null";
}

void CameraProcess::binaryFrameReady(const QImage &frame)
{
    // 将二值化帧传递给 QML
    qDebug() << "binaryFrameReady";

    // if(imageProvider_binary)
    //     imageProvider_binary->setImage(frame);
    // else
    //     qDebug() << "imageProvider_binary is null";
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
