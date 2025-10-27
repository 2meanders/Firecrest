#include "Maths.h"
#include <cmath>

float fc::maths::map(float value, float fromMin, float fromMax, float toMin, float toMax) {
    return (value - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

float fc::maths::abs(float value) {
    if (value < 0)
        return value * -1.0f;
    return value;
}

float fc::maths::round(float n, int decimals) {
    int a = static_cast<int>(pow(10, decimals));
    return std::round(n * a) / a;
}