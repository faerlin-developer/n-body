#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <exception>

class Timer {

private:

    std::map<std::string, sf::Clock> clocks;
    std::map<std::string, float> elapsed;

public:

    Timer() = default;

    void start(const std::string &key) {

        if (clocks.find(key) == clocks.end()) {
            clocks[key] = sf::Clock();
        }

        clocks[key].restart();
    }

    void end(const std::string &key) {

        if (clocks.find(key) == clocks.end()) {
            throw std::invalid_argument("key does not exist");
        }

        elapsed[key] = clocks[key].getElapsedTime().asSeconds();
    }

    float getElapsed(const std::string &key) {
        return elapsed[key];
    }

};