#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <variant>
#include "entityManager.h"
#include "../Entities/utils/entitiesManager.hpp"
#include "../Entities/utils/component.hpp"

class SfmlRenderer
{
private:
public:
    void render(sf::RenderWindow& window, EntitiesManager& manager, TextureLoader& textureLoader);
};


