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

int GameOverLable(EntitiesManager& manager, TextureLoader& textureLoader, std::string className) {
    manager.addEntity(className);

    SizeComponent size(74*5.41, 74);
    manager.addComponent<SizeComponent>(size);

    PositionComponent pos(WINDOW_WIDTH/2 - size.size.x / 2, WINDOW_HEIGHT/2 - size.size.y / 2);
    manager.addComponent<PositionComponent>(pos);


    StateComponent state((int)LableState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)LableState::idle, {
                {"Store/view/Lable/GameOver.png", 1.f, false},
            }},
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);
    return manager.getID();
}

int YouWinLable(EntitiesManager& manager, TextureLoader& textureLoader, std::string className) {
    manager.addEntity(className);

    SizeComponent size(74*3.9, 74);
    manager.addComponent<SizeComponent>(size);

    PositionComponent pos(WINDOW_WIDTH/2 - size.size.x / 2, WINDOW_HEIGHT/2 - size.size.y / 2);
    manager.addComponent<PositionComponent>(pos);

    StateComponent state((int)LableState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)LableState::idle, {
                {"Store/view/Lable/YouWin.png", 1.f, false},
            }},
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);
    return manager.getID();
}