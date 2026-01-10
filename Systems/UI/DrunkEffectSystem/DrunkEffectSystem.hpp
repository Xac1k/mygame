#pragma once
#include <SFML/Graphics.hpp>

class DrunkEffectSystem
{
private:
    float drunkLevel = 0.f;
    sf::Clock clockForDrunkShrinking;
    static constexpr float DurationOfDrunkEffect = 120.f;
    bool firstActivation = true;
public:
    void update() {
        if (drunkLevel != 0  && firstActivation) {
            firstActivation = false;
            clockForDrunkShrinking.restart();
        }
        else if(!firstActivation && drunkLevel > 0) {
            drunkLevel -= clockForDrunkShrinking.restart().asSeconds() / DurationOfDrunkEffect;
            // std::cout << "Минусуем уровень пьянства на " << clockForDrunkShrinking.restart().asSeconds() / DurationOfDrunkEffect 
            // << " Нынешний уровень:" << drunkLevel <<'\n';
            if(drunkLevel < 0) {
                drunkLevel = 0;
                firstActivation = true;
            }
        }
    }

    float getDrunkLevel() {
        return drunkLevel;
    }

    void addDrunkLevel(float increment) {
        drunkLevel += increment;
        if(drunkLevel > 1) drunkLevel = 1;
    };
};