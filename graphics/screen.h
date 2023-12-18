#pragma once

#include <SFML/Graphics.hpp>

class Screen {

public:

    static const int DEFAULT_FRAME_RATE = 24;

    sf::RenderWindow *window;

    Screen() = delete;

    Screen(int width, int height, int frameRate = DEFAULT_FRAME_RATE) {
        window = new sf::RenderWindow(sf::VideoMode(width, height), "N Body Simulation");
        window->setFramerateLimit(frameRate);
        centerWindow();
        centerView();
    }

    bool isOpen() {
        return window->isOpen();
    }

    /**
     * @return true if event is close.
     */
    bool pollEvent() {

        sf::Event event{};
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return true;
            }
        }

        return false;
    }

    void clear() {
        window->clear();
    }

    void display() {
        window->display();
    }

    ~Screen() {
        window->close();
        delete window;
    }

private:

    void centerWindow() {
        auto desktop = sf::VideoMode::getDesktopMode();
        auto xTopLeft = static_cast<int>(desktop.width / 2 - window->getSize().x / 2);
        auto yTopLeft = static_cast<int>(desktop.height / 2 - window->getSize().y / 2);
        auto pos = sf::Vector2i(xTopLeft, yTopLeft);
        window->setPosition(pos);
    }

    void centerView() {
        auto width = static_cast<float>(window->getSize().x);
        auto height = static_cast<float>(window->getSize().y);
        auto size = sf::Vector2f(width, height);
        auto center = sf::Vector2f(0, 0);
        sf::View view(center, size);
        window->setView(view);
    }

};