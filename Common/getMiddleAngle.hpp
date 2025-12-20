#pragma once
#include <cmath>

inline float normalizeAngle360(float angle) {
    angle = fmodf(angle, 360.0f);
    if (angle < 0.0f) angle += 360.0f;
    return angle;
}

inline float angleMidpoint(float a, float b) {
    a = normalizeAngle360(a);
    b = normalizeAngle360(b);

    float diff = fmodf(b - a + 360.0f, 360.0f);
    if (diff > 180.0f) {
        diff -= 360.0f;
    }

    float midpoint = a + diff / 2.0f;
    return normalizeAngle360(midpoint);
}