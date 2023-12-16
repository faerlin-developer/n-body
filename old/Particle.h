#pragma once

#include <cmath>
#include "State.h"

class Particle {
public:
    float mass;
    float radius;
    Vector pos;
    Vector vel;
    Vector acc;

    Particle() : Particle(1, 0, 0, 0, 0) {}

    Particle(float mass, float x, float y, float vx, float vy)
            : mass(mass),
              radius(std::pow(mass, 0.333)),
              pos(x, y),
              vel(vx, vy),
              acc(0, 0) {}

    Particle(Particle &other) {
        this->mass = other.mass;
        this->radius = other.radius;
        this->pos = other.pos;
        this->vel = other.vel;
        this->acc = other.acc;
    }

    Particle &operator=(const Particle &other) {

        if (this == &other) {
            return *this;
        }

        this->mass = other.mass;
        this->radius = other.radius;
        this->pos = other.pos;
        this->vel = other.vel;
        this->acc = other.acc;

        return *this;
    }

    // update the acceleration of this particle due to the gravitational influence of other
    // Needs to constraint minimum value of distance to prevent divergence on the value of the magnitude
    void attractedBy(Particle &other) {
        const float G = 0.1;
        auto acc = other.pos - this->pos;
        const auto magnitude = G * other.mass / std::min(std::max(10.f, acc.magnitudeSquared()), 1000.f);
        acc.setMagnitude(magnitude);
        this->acc = this->acc + acc;
    }

};
