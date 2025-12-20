inline float normAngle(float  angle) {
    if(angle > 360) angle -= 360;
    if(angle < 0) angle += 360;

    return angle;
}


