#include "entityManager.h"
#include <Entities/utils/component.hpp>

bool TextureLoader::isLoaded(const std::string& filename) {
    return buff.find(filename) != buff.cend();
}

void TextureLoader::loadFromFile(const std::string& filename) {
    sf::Sprite sprite;
    sf::Texture texture;

    texture.setSmooth(false);
    texture.setRepeated(false);
    std::string fullpath = buildFullPath(filename, 1);

    if (!texture.loadFromFile(fullpath)) { std::cout << "Невозможно открыть спрайт" << std::endl; }
    buff.insert(std::make_pair(fullpath, texture));
}

sf::Sprite& TextureLoader::getSprite(const std::string& filename, Vect2D pos, Vect2D size) {
    if(!isLoaded(filename)) loadFromFile(filename);

    sprite = sf::Sprite();

    std::string fullpath = buildFullPath(filename, 1);
    sprite.setTexture(buff[fullpath]);

    sf::Vector2u textureSize = buff[fullpath].getSize();
    float scaleX = size.x / textureSize.x;
    float scaleY = size.y / textureSize.y;
    
    sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
    sprite.setPosition(pos.x, pos.y);
    sprite.scale(sf::Vector2f({scaleX, scaleY}));
    //sprite.setOrigin({20/2, 20/2});

    return sprite;
}

sf::Texture TextureLoader::getTexture(const std::string &filename) {
    if(!isLoaded(filename)) loadFromFile(filename);

    std::string fullpath = buildFullPath(filename, 1);
    return buff[fullpath];
}

sf::Sprite& TextureLoader::getSprite(const FrameOnGrid& frame, Vect2D pos, Vect2D size, Vect2D tileSize) {
    if(!isLoaded(frame.path)) loadFromFile(frame.path);

    sprite = sf::Sprite();

    std::string fullpath = buildFullPath(frame.path, 1);
    sprite.setTexture(buff[fullpath]);

    float scaleX = size.x / tileSize.x * frame.mirror.x;
    float scaleY = size.y / tileSize.y * frame.mirror.y;
    
    sprite.setTextureRect(sf::IntRect(tileSize.x * frame.cellID.x, tileSize.y * frame.cellID.y, tileSize.x, tileSize.y));
    Vect2D mirrorSpriteCorrection(frame.mirror.x < 0 ? size.x : 0, frame.mirror.y < 0 ? size.y : 0);
    sprite.setPosition(pos.x + mirrorSpriteCorrection.x, pos.y + mirrorSpriteCorrection.y);
    sprite.scale(sf::Vector2f({scaleX, scaleY}));

    return sprite;
}

sf::Sprite& TextureLoader::getSprite(const FrameOnGrid& frame, Vect2D pos, Vect2D size, Vect2D tileSize, float alpha) {
   auto& sprite = getSprite(frame, pos, size, tileSize);

   auto color = sprite.getColor();
   color.a = alpha / 100 * 255;
   sprite.setColor(color);

   return sprite;
}
