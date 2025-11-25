#pragma once
#include <math.h>

int randRange(int minVal, int maxVal) {
    return minVal + rand() % (maxVal - minVal + 1);
}
