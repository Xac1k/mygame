#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include "../Common/Vect.hpp"

class BusEvent
{
private: 
public:
    enum class Event {
        MouseDownLeft, MouseDownRight, MouseMove, MouseUp, A, W, S, D, I, Space, None,
        Num0, Num1, Num2, Num3, Num4, Num5
    };
    Event currEvent;
    Vect2D mousePos;
    bool shiftPressed = false;
    bool ctrlPressed = false;
    bool altPressed = false;

    void update(sf::Event Event);
    int addEventListenre(Event event, std::function<void(BusEvent&)> callback);
};

using EventHandler = std::function<void(BusEvent&)>;

