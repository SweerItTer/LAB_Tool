// ColorConverter.cpp
#include "ColorConverter.h"

void ColorUtils::RGB2LAB(float r, float g, float b, float *LAB)
{
    cv::Mat rgbMat(1, 1, CV_32FC3);
    rgbMat.at<cv::Vec3f>(0, 0) = cv::Vec3f(r, g, b);

    cv::Mat labMat;
    cv::cvtColor(rgbMat, labMat, cv::COLOR_RGB2Lab);

    cv::Vec3f lab = labMat.at<cv::Vec3f>(0, 0);
    LAB[0] = lab[0];
    LAB[1] = lab[1];
    LAB[2] = lab[2];
}

void ColorUtils::RGB2LAB(const QColor &color, float *LAB)
{
    float r = color.redF();
    float g = color.greenF();
    float b = color.blueF();
    RGB2LAB(r, g, b, LAB);
}
