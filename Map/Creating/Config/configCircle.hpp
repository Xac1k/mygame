#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

constexpr int WIDTH_MAP_CIRCLE = 700;
constexpr int HEIGHT_MAP_CIRCLE = 700;

constexpr int MAX_NUM_ROOMS_CIRCLE = 10;
constexpr int MIN_NUM_ROOMS_CIRCLE = 6;
constexpr int MAX_RADIUS_ROOM_CIRCLE = 15;
constexpr int MIN_RADIUS_ROOM_CIRCLE = 10;
constexpr int INTERSECT_ROOMS_CIRCLE = 10;

constexpr int MAX_LEN = 200;
constexpr int MIN_LEN = 100;

using roomsType = std::vector<sf::Vector3f>;
using spiralType = std::vector<sf::Vector2f>;