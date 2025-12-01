#pragma once
#include <cmath>

class Vect2D
{
public:
    float x;
    float y;
    
    constexpr Vect2D(float xI = 0, float yI = 0) : x(xI), y(yI) {};

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

    void operator+=(const Vect2D& other) {
        this->x += other.x;
        this->y += other.y;
    };

    void operator-=(const Vect2D& other) {
        this->x -= other.x;
        this->y -= other.y;
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

    bool operator>(const Vect2D& other) {
        return x > other.x && y > other.y;
    };

    bool operator>(const int& num) {
        return x > num && y > num;
    };

    bool operator<(const int& num) {
        return x < num && y < num;
    };

    bool operator<(const Vect2D& other) {
        return x < other.x && y < other.y;
    };
};

inline Vect2D fromAngle(float angleDeg) {
    float x = sin(angleDeg / 180 * M_PI);
    float y = cos(angleDeg / 180 * M_PI);
    return Vect2D (x, y);
}