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