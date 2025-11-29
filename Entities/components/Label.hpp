#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"

enum class LableState {idle};
int optionLable(EntitiesManager& manager, TextureLoader& textureLoader, std::string className) {
    manager.addEntity(className);

    PositionComponent pos(250, 60);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(200, 77);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int)LableState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)LableState::idle, {
                {"Store/view/Lable/OptionsScaled.png", 1.f, false},
            }},
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);
    return manager.getID();
}

int volumeLable(EntitiesManager& manager, TextureLoader& textureLoader, std::string className) {
    manager.addEntity(className);

    PositionComponent pos(275, 137);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(150, 45);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int)LableState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)LableState::idle, {
                {"Store/view/Lable/VolumeScaled.png", 1.f, false},
            }},
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);

    return manager.getID();
}