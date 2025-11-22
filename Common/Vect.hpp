#pragma once
#include <cmath>  // ← лучше использовать <cmath> вместо "math.h"

class Vect2D
{
public:
    float x;
    float y;
    
    Vect2D(float xI = 0, float yI = 0): x(xI), y(yI) {};

    float length() const {
        return std::sqrt(x*x + y*y);
    };

    Vect2D operator-(const Vect2D& other) const {
        return Vect2D(x - other.x, y - other.y);
    };

    Vect2D operator-() const {
        return Vect2D(-x, -y);
    };

    Vect2D operator+(const Vect2D& other) const {
        return Vect2D(x + other.x, y + other.y);
    };

    Vect2D operator*(const Vect2D& other) const {
        return Vect2D(x * other.x, y * other.y);
    };

    Vect2D operator*(int other) const {
        return Vect2D(x * other, y * other);
    };

    Vect2D operator*(double other) const {
        return Vect2D(x * other, y * other);
    };

    Vect2D operator*(float other) const {
        return Vect2D(x * other, y * other);
    };

    Vect2D operator/(int other) const {
        return Vect2D(x / other, y / other);
    };

    

    Vect2D& operator=(const Vect2D& other) {
        x = other.x; 
        y = other.y;
        return *this;
    };
};