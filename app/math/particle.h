#pragma once

#include <cmath>
#include "math/state.h"
#include "math/region.h"

class Particle {

private:

    constexpr static float RADIUS_EXPONENT = 0.333;

public:

    float mass;
    float radius;
    Vector pos;
    Vector vel;
    Vector acc;

    Particle();

    Particle(float mass, float x, float y, float vx, float vy);

    Particle(Particle &other);

    Particle &operator=(const Particle &other);

    void attractedBy(Particle &other);

    float dist(float x, float y) const;

    static Region::Rectangle getConvexRegion(Particle *particles, int size);

};
