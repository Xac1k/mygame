#pragma once
#include <cmath>
#include <Common/Vect.hpp>

inline float toDegrees(float angleRad) {
    return angleRad / M_PI * 180;
}

inline float getAngle(Vect2D vect) {
    float angleDeg = toDegrees(atan2(vect.y, vect.x));
    if(angleDeg < 0) angleDeg += 360;
    return angleDeg;
}