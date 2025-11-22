#include "entityManager.h"

std::string buildPath(std::string path, int levelsUp = 0) {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path targetPath = currentPath;
    
    for (int i = 0; i < levelsUp; i++) {
        targetPath = targetPath.parent_path();
    }
    
    targetPath /= path;
    return targetPath.string();
}

bool TextureLoader::isLoaded(const std::string& filename) {
    return buff.find(filename) != buff.cend();
}

void TextureLoader::loadFromFile(const std::string& filename) {
    sf::Sprite sprite;
    sf::Texture texture;

    texture.setSmooth(false);
    texture.setRepeated(false);
    std::string fullpath = buildPath(filename, 1);

    if (!texture.loadFromFile(fullpath)) { std::cout << "Невозможно открыть спрайт" << std::endl; }
    buff.insert(std::make_pair(fullpath, texture));
}

sf::Sprite& TextureLoader::getSprite(const std::string& filename, Vect2D pos, Vect2D size) {
    if(!isLoaded(filename)) loadFromFile(filename);

    sprite = sf::Sprite();

    std::string fullpath = buildPath(filename, 1);
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
