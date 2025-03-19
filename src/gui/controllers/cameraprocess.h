#ifndef CAMERAPROCESS_H
#define CAMERAPROCESS_H

#include <QObject>
#include <QImage>
#include <QList>
#include <QCameraInfo>

// 视频处理(二值化) 
class CameraManager; // 前置声明

class CameraProcess : public QObject {
    Q_OBJECT

public:
    // 将构造函数声明和实现分离
    explicit CameraProcess(QObject *parent = nullptr);
    ~CameraProcess();

public slots:
    void check(); // 槽函数
    void startProcess(int index); // 开始处理的槽函数
    void stopProcess();
signals:
    // 摄像头信息, 标志(0: 单个摄像头, 1: 多个摄像头)
    void checkWarning(QStringList cameraNames, int index);
    void checkError(); // 报错(无摄像头)

private:
    CameraManager *m_cameraManager = nullptr; // OpenCV 处理核心
};

#endif // !CAMERAPROCESS_H