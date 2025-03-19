#include <QSettings>
#include <QFile>

#include "dataprocess.h"
#include "src/core/ColorConverter.h"

using namespace ColorUtils;

void DataProcess::processRGB(const QColor &color)
{
    // LAB转换
    float labArray[3];
    ColorUtils::RGB2LAB(color, labArray);
    // 这里暂时直接转发原始数据
    emit rgbDataProcessed(labArray[0], labArray[1], labArray[2]);
}

// dataprocess.cpp
void DataProcess::colorChanged(int index, const QString &colorName,
    const QVariantList &l,
    const QVariantList &a,
    const QVariantList &b) 
{
    // 边界检查
    if (index < 0 || index >= configMap.size()) {
        qWarning() << "Invalid config index:" << index;
        return;
    }
    if (colorName.isEmpty()) {
        qWarning() << "Empty color name";
        return;
    }

    // 数据有效性检查
    auto convertToIntList = [](const QVariantList &varList) -> QVector<int> {
        QVector<int> list;
        for (const QVariant &v : varList) {
            bool ok;
            int value = v.toInt(&ok);
            if (!ok) {
                qWarning() << "Invalid value in list:" << v;
                return QVector<int>();
            }
            list.append(value);
        }
        return list;
    };

    QVector<int> lValues = convertToIntList(l);
    QVector<int> aValues = convertToIntList(a);
    QVector<int> bValues = convertToIntList(b);

    if (lValues.size() != 2 || aValues.size() != 2 || bValues.size() != 2) {
        qWarning() << "Invalid LAB values for color:" << colorName;
        return;
    }

    // 更新配置
    LABRange range;
    range.L = lValues;
    range.A = aValues;
    range.B = bValues;
    configMap[index][colorName] = range;

    // 可选：立即保存到文件
    saveConfig(configFilePath);
}

QVariantMap DataProcess::getColor(int index, const QString &colorName)
{
    QVariantMap result;
    // 检查索引是否有效
    if (index < 0 || index >= 5) {
        qWarning() << "Invalid index:" << index;
        return result;
    }
    const QMap<QString, LABRange> &colorMap = configMap[index];
    if (!colorMap.contains(colorName)) {
        qWarning() << "Color not found:" << colorName;
        return result;
    }
    const LABRange &range = colorMap[colorName];
    result["L"] = QVariantList{range.L[0], range.L[1]};
    result["A"] = QVariantList{range.A[0], range.A[1]};
    result["B"] = QVariantList{range.B[0], range.B[1]};
    return result;
}

#include <QSettings>
#include <QDebug>

void DataProcess::loadconfig() {
    // configMap.clear(); // 清空旧配置

    if (configFilePath.isEmpty() || !QFile::exists(configFilePath))
    {
        qDebug() << "Config file not found.";
        return;
    }
    
    QSettings settings(configFilePath, QSettings::IniFormat);
    
    // 1. 获取所有分组
    const QStringList groups = settings.childGroups();
    if(groups.size() != 5){
        qDebug() << "No config found.";
        return;
    }
    for (const QString &group : groups) {
        settings.beginGroup(group);
        
        QMap<QString, LABRange> colorMap;
        
        // 2. 获取当前分组下的所有键（例如 red_L, red_A）
        const QStringList keys = settings.childKeys();
        for (const QString &key : keys) {
            // 3. 解析键名：将 "red_L" 分解为 color="red", channel="L"
            QStringList parts = key.split('_');
            if (parts.size() != 2) {
                qWarning() << "Invalid key format:" << key;
                continue;
            }
            
            QString color = parts[0].toLower(); // 统一小写
            QString channel = parts[1].toUpper(); // 统一大写
            
            // 4. 解析值：将 "51,164" 转换为 [51, 164]
            QString valueStr = settings.value(key).toString();
            QStringList valueParts = valueStr.split(',');
            if (valueParts.size() != 2) {
                qWarning() << "Invalid value format for key:" << key;
                continue;
            }
            
            bool ok1, ok2;
            int min = valueParts[0].toInt(&ok1);
            int max = valueParts[1].toInt(&ok2);
            if (!ok1 || !ok2) {
                qWarning() << "Non-integer value in key:" << key;
                continue;
            }
            
            // 5. 填充 LABRange 结构
            LABRange &range = colorMap[color]; // 自动创建或获取现有条目
            if (channel == "L") {
                range.L = QVector<int>{min, max};
            } else if (channel == "A") {
                range.A = QVector<int>{min, max};
            } else if (channel == "B") {
                range.B = QVector<int>{min, max};
            } else {
                qWarning() << "Unknown channel:" << channel;
            }
        }
        
        settings.endGroup();
        configMap.append(colorMap);
    }
    
    // 如果 INI 文件为空，初始化默认配置
    if (configMap.isEmpty()) {
        qDebug() << "No config found.";
    }
}

void DataProcess::saveRequested(){
    for(int i = 0; i < configMap.size(); i++) {
        if(configMap[i].isEmpty()){
            emit errorOccurred("Config is not ready.");
            return;
        }
    }
    emit saveSuccess();
    saveConfig(configFilePath);
}

void DataProcess::saveConfig(const QString &filePath) {
    QSettings settings(filePath, QSettings::IniFormat);
    settings.clear();
    // 遍历 configMap，将每个颜色的 LABRange 写入 INI 文件
    for (int i = 0; i < configMap.size(); ++i) {
        const QMap<QString, LABRange> &colorMap = configMap[i];
        for (auto it = colorMap.begin(); it != colorMap.end(); ++it) {
            const QString &color = it.key();
            const LABRange &range = it.value();
            settings.beginGroup(QString("%1").arg(color));
            settings.setValue(color + "_L", QString("%1,%2").arg(range.L[0]).arg(range.L[1]));
            settings.setValue(color + "_A", QString("%1,%2").arg(range.A[0]).arg(range.A[1]));
            settings.setValue(color + "_B", QString("%1,%2").arg(range.B[0]).arg(range.B[1]));
        }
        settings.endGroup();
    }
    settings.sync();
}
