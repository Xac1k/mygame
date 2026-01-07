#include "entityManager.h"
#include <Entities/utils/component.hpp>

bool TextureLoader::isLoaded(const std::string& filename) {
    return buff.find(filename) != buff.cend();
}

void TextureLoader::loadFromFile(const std::string& filename) {
    std::cout << filename << std::endl;
    sf::Texture texture;

    texture.setSmooth(false);
    texture.setRepeated(false);
    std::string fullpath = buildFullPath(filename, 1);

    if (!texture.loadFromFile(fullpath)) { std::cout << "Невозможно открыть спрайт" << std::endl; }
    buff.insert(std::make_pair(filename, texture));
}

void resetAlpha(sf::Sprite& sprite) {
    auto color = sprite.getColor();
   color.a = 255;
   sprite.setColor(color);
}

sf::Sprite& TextureLoader::getSprite(const std::string& filename, Vect2D pos, Vect2D size) {
    if(!isLoaded(filename)) loadFromFile(filename);
    resetAlpha(sprite);

    sprite.setTexture(buff[filename]);

    sf::Vector2u textureSize = buff[filename].getSize();
    float scaleX = size.x / textureSize.x;
    float scaleY = size.y / textureSize.y;
    
    sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
    sprite.setPosition(pos.x, pos.y);
    sprite.setScale(sf::Vector2f({scaleX, scaleY}));

    return sprite;
}

sf::Sprite& TextureLoader::getSprite(const std::string& filename, Vect2D pos, Vect2D size, Vect2D tileSize, Vect2D tileID, Vect2D mirror) {
   if(!isLoaded(filename)) loadFromFile(filename);

    sprite.setTexture(buff[filename]);

    float scaleX = size.x / tileSize.x * mirror.x;
    float scaleY = size.y / tileSize.y * mirror.y;
    
    sprite.setTextureRect(sf::IntRect(tileSize.x * tileID.x, tileSize.y * tileID.y, tileSize.x, tileSize.y));
    Vect2D mirrorSpriteCorrection(mirror.x < 0 ? size.x : 0, mirror.y < 0 ? size.y : 0);
    sprite.setPosition(pos.x + mirrorSpriteCorrection.x, pos.y + mirrorSpriteCorrection.y);
    sprite.setScale(sf::Vector2f({scaleX, scaleY}));

    return sprite;
}

sf::Texture TextureLoader::getTexture(const std::string &filename) {
    if(!isLoaded(filename)) loadFromFile(filename);

    return buff[filename];
}

sf::Sprite& TextureLoader::getSprite(const FrameOnGrid& frame, Vect2D pos, Vect2D size, Vect2D tileSize) {
    if(!isLoaded(frame.path)) loadFromFile(frame.path);
    resetAlpha(sprite);

    sprite.setTexture(buff[frame.path]);

    float scaleX = size.x / tileSize.x * frame.mirror.x;
    float scaleY = size.y / tileSize.y * frame.mirror.y;
    
    sprite.setTextureRect(sf::IntRect(tileSize.x * frame.cellID.x, tileSize.y * frame.cellID.y, tileSize.x, tileSize.y));
    Vect2D mirrorSpriteCorrection(frame.mirror.x < 0 ? size.x : 0, frame.mirror.y < 0 ? size.y : 0);
    sprite.setPosition(pos.x + mirrorSpriteCorrection.x, pos.y + mirrorSpriteCorrection.y);
    sprite.setScale(sf::Vector2f({scaleX, scaleY}));

    return sprite;
}

sf::Sprite& TextureLoader::getSprite(const FrameOnGrid& frame, Vect2D pos, Vect2D size, Vect2D tileSize, float alpha) {
   auto& sprite = getSprite(frame, pos, size, tileSize);

   auto color = sprite.getColor();
   color.a = alpha / 100 * 255;
   sprite.setColor(color);

   return sprite;
}
