/*
qml 显示滑条 LAB 范围
实时获取 LAB 值 (用户调整)
*/
#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <QObject>
#include <QColor>

class DataProcess : public QObject {
    Q_OBJECT

public:
    explicit DataProcess(QObject *parent = nullptr){}

public slots:
    void processRGB(const QColor &color);  // 处理RGB的槽函数
    void LRange(float maxL, float minL); // 获取LAB范围的槽函数
    void ARange(float maxA, float minA); 
    void BRange(float maxB, float minB); 

signals:
    void rgbDataProcessed(float L, float A, float B); // 处理完成信号
};

#endif // !DATAPROCESS_H