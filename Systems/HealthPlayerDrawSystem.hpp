#pragma once
#include <Entities/components/map.hpp>
#include <Entities/components/player.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <SFML/Graphics.hpp>
#include <Sounds/soundManager.hpp>
#include <Interfaces/busEvent.h>
#include <Common/getAngle.hpp>
#include <Map/Creating/Common/dist.hpp>


class HealthPlayerLevel {
    private:
        static constexpr Vect2D marginPreview = Vect2D(34, 10);

    public: 
        Vect2D margin = Vect2D(10, 10);
        Vect2D size = Vect2D(120, 35);

    void draw(sf::RenderWindow &window, TextureLoader &textureLoader, EntitiesManager& manager) {
        auto playerIds = manager.withClassName("*player*");
        if(playerIds.size() == 0) return;
        auto playHealthCom = manager.getComponent<HealthComponent>(playerIds[0]).get();

        auto HealthBarSprite = textureLoader.getSprite("Store/view/HealthBar/HealthBar.png", margin, size);
        window.draw(HealthBarSprite);

        for(int i = 0; i < playHealthCom->health * 10 / playHealthCom->maxHealth; i++) {
            auto FullCell = textureLoader.getSprite("Store/view/HealthBar/LevelHigh.png", 
                Vect2D(margin.x + marginPreview.x + 8 * i, margin.y + marginPreview.y), 
                Vect2D(8, 10));
            window.draw(FullCell);
        }
    }
};

