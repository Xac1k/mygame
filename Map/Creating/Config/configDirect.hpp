#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

constexpr int WIDTH_MAP = 700;
constexpr int HEIGHT_MAP = 700;

constexpr int MAX_NUM_ROOMS = 10;
constexpr int MIN_NUM_ROOMS = 4;
constexpr int MAX_RADIUS_ROOM = 35;
constexpr int MIN_RADIUS_ROOM = 10;
constexpr int MAX_ATTEMPTS = 2000;

constexpr int INTERSECT_ROOMS = 10;

using roomsType = std::vector<sf::Vector3f>;
using corridorsType = std::vector<std::pair<int, int>>;