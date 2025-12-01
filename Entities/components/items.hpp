#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"
#include "../../main.h"

int item(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D posOnMap) {
    manager.addEntity("playMenu:Enemy:Item", true);

    PositionOnMapComponent posMap(posOnMap);
    manager.addComponent<PositionOnMapComponent>(posMap);

    SizeComponent size(32, 32);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int) Items::none);
    manager.addComponent<StateComponent>(state);

    PickUpItemComponent itemComp;
    manager.addComponent<PickUpItemComponent>(itemComp);

    AnimationGridComponent animationComponent;
    animationComponent.TileSizeInGrid = {32, 32};
    loadAnimations(animationComponent.animation, 
        {
            {(int)Items::coin, {
                {"Store/view/Items/item13.png", {0, 0}, 1.f, true},
            }},
        }
    );
    manager.addComponent<AnimationGridComponent>(animationComponent);

    return manager.getID();
}