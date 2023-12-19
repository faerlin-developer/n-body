
#include <SFML/Graphics.hpp>
#include "graphics/screen.h"

Screen::Screen(int width, int height, int frameRate) {
    window = new sf::RenderWindow(sf::VideoMode(width, height), "N Body Simulation");
    window->setFramerateLimit(frameRate);
    centerWindow();
    centerView();
}

bool Screen::isOpen() const {
    return window->isOpen();
}

bool Screen::pollEvent() const {

    sf::Event event{};
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            return true;
        }
    }

    return false;
}

void Screen::clear() const {
    window->clear();
}

void Screen::display() const {
    window->display();
}

Screen::~Screen() {
    window->close();
    delete window;
}

void Screen::centerWindow() const {
    auto desktop = sf::VideoMode::getDesktopMode();
    auto xTopLeft = static_cast<int>(desktop.width / 2 - window->getSize().x / 2);
    auto yTopLeft = static_cast<int>(desktop.height / 2 - window->getSize().y / 2);
    auto pos = sf::Vector2i(xTopLeft, yTopLeft);
    window->setPosition(pos);
}

void Screen::centerView() const {
    auto width = static_cast<float>(window->getSize().x);
    auto height = static_cast<float>(window->getSize().y);
    auto size = sf::Vector2f(width, height);
    auto center = sf::Vector2f(0, 0);
    sf::View view(center, size);
    window->setView(view);
}