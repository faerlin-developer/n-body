
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "graphics/timer.h"

void Timer::start(const std::string &key) {

    if (clocks.find(key) == clocks.end()) {
        clocks[key] = sf::Clock();
    }

    clocks[key].restart();
}

void Timer::end(const std::string &key) {

    if (clocks.find(key) == clocks.end()) {
        throw std::invalid_argument("key does not exist");
    }

    elapsed[key] = clocks[key].getElapsedTime().asSeconds();
}

float Timer::getElapsed(const std::string &key) {
    return elapsed[key];
}