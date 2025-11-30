#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <Common/Vect.hpp>

inline float dist(Vect2D point0, Vect2D point1) {
    Vect2D vect = point1 - point0;
    return std::sqrt(vect.x * vect.x + vect.y * vect.y);
}

inline float dist(sf::Vector2f point0, sf::Vector2f point1) {
    sf::Vector2f vect = point1 - point0;
    return std::sqrt(vect.x * vect.x + vect.y * vect.y);
}

inline float dist(sf::Vector3f point0, sf::Vector3f point1) {
    sf::Vector3f vect = point1 - point0;
    return std::sqrt(vect.x * vect.x + vect.y * vect.y);
}
