#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"
#include "../../main.h"

enum class BackgroundState { idle };
void background(EntitiesManager& manager, TextureLoader& textureLoader) {
    manager.addEntity();

    PositionComponent pos(0, 0);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(WINDOW_WIDTH, WINDOW_HEIGHT);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int) BackgroundState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)BackgroundState::idle, {
                {"Store/view/Background/sprites/image.png", 5.f, false},
                {"Store/view/Background/sprites/startmenu-1.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-2.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-3.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-4.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-5.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-6.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-7.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-8.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-9.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-10.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-11.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-12.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-13.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-14.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-15.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-16.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-17.png", 0.25f, false},
                {"Store/view/Background/sprites/startmenu-18.png", 0.25f, true},
            }},
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);
}