#include "dataprocess.h"
#include "src\core\ColorConverter.h"

using namespace ColorUtils;

void DataProcess::processRGB(const QColor &color)
{
    // LAB转换
    float labArray[3];
    ColorUtils::RGB2LAB(color, labArray);
    // 这里暂时直接转发原始数据
    emit rgbDataProcessed(labArray[0], labArray[1], labArray[2]);
}

void DataProcess::LRange(float maxL, float minL)
{
}

void DataProcess::ARange(float maxA, float minA)
{
}

void DataProcess::BRange(float maxB, float minB)
{
}