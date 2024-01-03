#pragma once

#include <SFML/Graphics.hpp>

class Screen {

public:

    static const int DEFAULT_FRAME_RATE = 24;

    sf::RenderWindow *window;

    Screen() = delete;

    Screen(int width, int height, int frameRate = DEFAULT_FRAME_RATE);

    bool isOpen() const;

    bool pollEvent() const;

    void clear() const;

    void display() const;

    ~Screen();

private:

    void centerWindow() const;

    void centerView() const;

};