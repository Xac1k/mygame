#include "sfml.h"

Frame getCurrentSprite(float time, Animation animation) {
    float resultTime = 0;
    for (auto sprite : animation) {
        resultTime += sprite.durationTime;
        if (time < resultTime) return sprite;
    }
    return animation[animation.size() - 1];
}

void SfmlRenderer::render(sf::RenderWindow &window, EntitiesManager &manager, TextureLoader &textureLoader) {
    auto entitiIDs = manager.with<AnimationComponent>().with<PositionComponent>().with<SizeComponent>().get();
    for(int id : entitiIDs) {
        auto animation = manager.getComponent<AnimationComponent>(id).get();
        auto state = manager.getComponent<StateComponent>(id).get()->state;
        auto pos = manager.getComponent<PositionComponent>(id).get()->point;
        auto size = manager.getComponent<SizeComponent>(id).get()->size;

        auto sprite = getCurrentSprite(animation->time, animation->animation[state]);
        sf::Sprite spriteD = textureLoader.getSprite(sprite.path, pos, size);
        window.draw(spriteD);
    } 
}