#pragma once

#include "math/particle.h"
#include "simulator/simulator.h"

class Naive : public Simulator {

public:

    void updateAcceleration(Particle *particles, int start, int end, int size) override;

    void updatePosition(Particle *particles, int start, int end, int size) override;

    void draw(Particle *particles, int size, sf::RenderWindow *window, bool drawRegion);

};