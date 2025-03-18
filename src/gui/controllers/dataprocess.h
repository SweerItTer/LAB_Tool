/*
qml 显示滑条 LAB 范围
实时获取 LAB 值 (用户调整)
*/
#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <QObject>
#include <QColor>
#include <QList>
#include <QDebug>
#include <QMap>
#include <QVector>

struct LABRange {
    QVector<int> L; // 改用QVector(qml 支持)
    QVector<int> A;
    QVector<int> B;
};

class DataProcess : public QObject {
    Q_OBJECT

public:
    explicit DataProcess(QObject *parent = nullptr)
    : QObject(parent), configFilePath("config.json")
    {
        configMap.resize(5);
    }
    ~DataProcess(){/* nothing */}
public slots:
    // 处理RGB
    void processRGB(const QColor &color);
    // 获取LAB范围
    Q_INVOKABLE void colorChanged(int index, const QString &colorName, 
        const QVariantList &l, 
        const QVariantList &a, 
        const QVariantList &b);
    // 设置LAB范围
    Q_INVOKABLE QVariantMap getColor(int index, const QString &colorName);
   
    // 加载配置
    void loadconfig(); // 加载配置请求槽函数
    void saveRequested(); // 保存配置请求槽函数
signals:
    void rgbDataProcessed(float L, float A, float B); // 处理完成信号
    void errorOccurred(const QString &message); // 错误信号
private:
    // 保存配置
    void saveConfig(const QString &filePath);
    // 存储LAB范围
    QVector<QMap<QString, LABRange>> configMap; // 动态数组 + QMap
    QString configFilePath; // 配置文件路径
};

#endif // !DATAPROCESS_H