#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <variant>
#include "entityManager.h"
#include "../Entities/utils/entitiesManager.hpp"
#include "../Entities/utils/component.hpp"

sf::Sprite getCurrentSprite(AnimationComponent* animationComponent, Vect2D pos, Vect2D size, TextureLoader &textureLoader, int state);
sf::Sprite getCurrentSprite(AnimationGridComponent* animationComponent, Vect2D pos, Vect2D size, TextureLoader &textureLoader, int state);
void drawGridAnimation(sf::RenderWindow &window, EntitiesManager &manager, TextureLoader &textureLoader, int id);
bool drawAnimationOneFile(sf::RenderWindow &window, EntitiesManager &manager, TextureLoader &textureLoader, int id);

class SfmlRenderer
{
private:
public:
    void render(sf::RenderWindow& window, EntitiesManager& manager, TextureLoader& textureLoader);
};


