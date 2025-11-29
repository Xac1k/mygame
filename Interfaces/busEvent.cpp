#include "busEvent.h"

void BusEvent::update(sf::Event event) {
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        mousePos.x = event.mouseButton.x;
        mousePos.y = event.mouseButton.y;
        switch (event.mouseButton.button) {
        case sf::Mouse::Left:
            currEvent = Event::MouseDownLeft;
            break;
        case sf::Mouse::Right:
            currEvent = Event::MouseDownRight;
            break;
        default:
            currEvent = Event::None;
            break;
        }
        break;

    case sf::Event::MouseMoved:
        currEvent = Event::MouseMove;
        mousePos.x = event.mouseMove.x;
        mousePos.y = event.mouseMove.y;
        break;

    case sf::Event::MouseButtonReleased:
        currEvent = Event::MouseUp;
        mousePos.x = event.mouseButton.x;
        mousePos.y = event.mouseButton.y;
        break;

    case sf::Event::KeyPressed:
        shiftPressed = event.key.shift;
        ctrlPressed = event.key.control;
        altPressed = event.key.alt;
        switch (event.key.code) {
        case sf::Keyboard::Key::A:
            currEvent = Event::A;
            break;
        case sf::Keyboard::Key::W:
            currEvent = Event::W;
            break;
        case sf::Keyboard::Key::S:
            currEvent = Event::S;
            break;
        case sf::Keyboard::Key::D:
            currEvent = Event::D;
            break;
        case sf::Keyboard::Key::I:
            currEvent = Event::I;
            break;
        case sf::Keyboard::Key::Num0:
            currEvent = Event::Num0;
            break;
        case sf::Keyboard::Key::Num1:
            currEvent = Event::Num1;
            break;
        case sf::Keyboard::Key::Num2:
            currEvent = Event::Num2;
            break;
        case sf::Keyboard::Key::Num3:
            currEvent = Event::Num3;
            break;
        case sf::Keyboard::Key::Num4:
            currEvent = Event::Num4;
            break;
        case sf::Keyboard::Key::Num5:
            currEvent = Event::Num5;
            break;
        case sf::Keyboard::Key::Escape:
            currEvent = Event::Esc;
            break;
        default:
            currEvent = Event::None;
            break;
        }
        break;

    default:
        currEvent = Event::None;
        break;
    }
}

int BusEvent::addEventListenre(Event event, std::function<void(BusEvent&)> callback) {
    if (event == currEvent) {
        callback(*this);
        return 1;
    }
    return 0;
}