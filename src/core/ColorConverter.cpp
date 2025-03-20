// ColorConverter.cpp
#include "ColorConverter.h"

// 0-255范围
void ColorUtils::RGB2LAB(float r, float g, float b, float *LAB)
{
    // 1. 创建 Mat 并存储 [0,255] 颜色值
    cv::Mat rgbMat(1, 1, CV_32FC3);
    rgbMat.at<cv::Vec3f>(0, 0) = cv::Vec3f(r * 255.0f, g * 255.0f, b * 255.0f);

    // 2. 转换到 8-bit 以防止精度丢失
    cv::Mat rgb8U;
    rgbMat.convertTo(rgb8U, CV_8UC3);

    // 3. 进行 RGB → LAB 颜色转换
    cv::Mat labMat;
    cv::cvtColor(rgb8U, labMat, cv::COLOR_RGB2Lab);

    // 4. 读取转换后的 LAB 结果
    cv::Vec3b lab = labMat.at<cv::Vec3b>(0, 0);
    LAB[0] = lab[0]; // L: 0-255
    LAB[1] = lab[1]; // A: 0-255
    LAB[2] = lab[2]; // B: 0-255
}


void ColorUtils::RGB2LAB(const QColor &color, float *LAB)
{
    float r = color.redF();
    float g = color.greenF();
    float b = color.blueF();
    RGB2LAB(r, g, b, LAB);
}
