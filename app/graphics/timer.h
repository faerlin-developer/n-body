#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class Timer {

private:

    std::map<std::string, sf::Clock> clocks;
    std::map<std::string, float> elapsed;

public:

    Timer() = default;

    void start(const std::string &key);

    void end(const std::string &key);

    float getElapsed(const std::string &key);

};