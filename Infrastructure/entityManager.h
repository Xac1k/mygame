#pragma once
#include <vector>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include "../Common/Vect.hpp"
#include "../Common/buildPath.hpp"
#include "../Entities/utils/component.hpp"

class TextureLoader {
private:
    std::map<std::string, sf::Texture> buff;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    bool isLoaded(const std::string& filename);
    void loadFromFile(const std::string& filename);
    sf::Texture getTexture(const std::string& filename);
    sf::Sprite& getSprite(const std::string& filename, Vect2D pos, Vect2D size);
    sf::Sprite& getSprite(const FrameOnGrid& frame, Vect2D pos, Vect2D size, Vect2D tileSize);
};