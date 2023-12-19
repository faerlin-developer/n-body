#pragma once

#include <SFML/Graphics.hpp>
#include "math/particle.h"
#include "args/args.h"


class Simulator {
public:

    virtual void updateAcceleration(Particle *particles, int start, int end, int size) = 0;

    virtual void updatePosition(Particle *particles, int start, int end, int size) = 0;

    virtual void draw(Particle *particles, int size, sf::RenderWindow *window, bool drawRegion) = 0;

    virtual ~Simulator() = default;

};
