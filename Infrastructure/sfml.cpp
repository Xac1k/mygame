#include "sfml.h"

sf::Sprite getCurrentSprite(AnimationComponent* animationComponent, Vect2D pos, Vect2D size, TextureLoader &textureLoader, int state) {
    float resultTime = 0;
    auto animation = animationComponent->animation[state];

    for (auto sprite : animation) {
        resultTime += sprite.durationTime;
        if (animationComponent->time < resultTime) 
            return textureLoader.getSprite(sprite.path, pos, size);
    }
    return textureLoader.getSprite(animation[animation.size() - 1].path, pos, size);
}

sf::Sprite getCurrentSprite(AnimationGridComponent* animationComponent, Vect2D pos, Vect2D size, TextureLoader &textureLoader, int state) {
    float resultTime = 0;
    auto animation = animationComponent->animation[state];

    for (auto sprite : animation) {
        resultTime += sprite.durationTime;
        if (animationComponent->time < resultTime) 
            return textureLoader.getSprite(sprite, pos, size, animationComponent->TileSizeInGrid);
    }
    
    return textureLoader.getSprite(animation[animation.size() - 1], pos, size, animationComponent->TileSizeInGrid);
}

void drawGridAnimation(sf::RenderWindow &window, EntitiesManager &manager, TextureLoader &textureLoader, int id) {
    auto animation = manager.getComponent<AnimationGridComponent>(id);

    auto state = manager.getComponent<StateComponent>(id).get()->state;
    auto pos = manager.getComponent<PositionComponent>(id).get()->point;
    auto size = manager.getComponent<SizeComponent>(id).get()->size;

    auto sprite = getCurrentSprite(animation.get(), pos, size, textureLoader, state);
    window.draw(sprite);
}

bool drawAnimationOneFile(sf::RenderWindow &window, EntitiesManager &manager, TextureLoader &textureLoader, int id) {
    auto animation = manager.getComponent<AnimationComponent>(id);
    if(!animation) return false;

    auto state = manager.getComponent<StateComponent>(id).get()->state;
    auto pos = manager.getComponent<PositionComponent>(id).get()->point;
    auto size = manager.getComponent<SizeComponent>(id).get()->size;

    auto sprite = getCurrentSprite(animation.get(), pos, size, textureLoader, state);
    window.draw(sprite);
    return true;
}

void SfmlRenderer::render(sf::RenderWindow &window, EntitiesManager &manager, TextureLoader &textureLoader) {
    auto entitiIDs = manager.with<AnimationComponent>().with<PositionComponent>().with<SizeComponent>().get();
    auto entitiIDsWithGrid = manager.with<AnimationGridComponent>().with<PositionComponent>().with<SizeComponent>().get();
    entitiIDs.insert( entitiIDs.end(), entitiIDsWithGrid.begin(), entitiIDsWithGrid.end());
    std::sort(entitiIDs.begin(), entitiIDs.end());

    for(int id : entitiIDs) {
        if(!drawAnimationOneFile(window, manager, textureLoader, id)) {
            drawGridAnimation(window, manager, textureLoader, id);
            continue;
        }
    } 
}