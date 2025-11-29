#pragma once
#include <math.h>

inline int randRange(int minVal, int maxVal) {
    return minVal + rand() % (maxVal - minVal + 1);
}
