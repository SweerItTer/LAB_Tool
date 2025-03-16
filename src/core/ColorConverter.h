#ifndef COLORCONVERTER_H
#define COLORCONVERTER_H

#include <string>
#include <vector>

#include <QColor>
#include <opencv2/opencv.hpp>

namespace ColorUtils {
    void RGB2LAB(float r, float g, float b, float *LAB);
    void RGB2LAB(const QColor &color, float *LAB);
}

#endif // COLORCONVERTERH
