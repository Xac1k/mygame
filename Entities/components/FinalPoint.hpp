#pragma once
#include <Common/const.hpp>
#include <Entities/utils/dialogComponent.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/utils/animationLoader.hpp>

enum class FinalPointState {idle};

void finalPoint(EntitiesManager& manager, Vect2D posOnMap, std::string localPath) {
    manager.addEntity("playMenu:FinalPoint Deletable Static", true);

    constexpr float scale = 1.25; 
    PositionOnMapComponent posMap(posOnMap);
    manager.addComponent<PositionOnMapComponent>(posMap);

    DialogTreeComponent dialogTree;
    dialogTree.loadFromTXT(localPath);
    manager.addComponent(dialogTree);

    SizeComponent size(Vect2D(64, 48) * scale);
    manager.addComponent(size);

    StateComponent state((int)FinalPointState::idle);
    manager.addComponent(state);

    AnimationGridComponent animationComponent;
    animationComponent.TileSizeInGrid = {95, 73};
    loadAnimations(animationComponent.animation, 
        {
            {(int)FinalPointState::idle, {
                {"Store/view/FinalPoint/image.png", {0, 0}, 1.f, true},
            }},
        }
    );
    manager.addComponent<AnimationGridComponent>(animationComponent);
}