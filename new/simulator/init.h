#pragma once

#include <random>
#include "math/particle.h"

namespace init {

    float random(float min, float max) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(mt);
    }

    void particles(Particle *particles, int size) {

        for (int i = 0; i < size; i++) {
            auto mass = random(5, 15);
            auto x = random(-1, 1);
            auto y = random(-1, 1);
            auto pos = Vector(x, y).unit();
            auto vel = pos;
            vel.rotate(M_PI / 2, Vector(0, 0));
            pos.setMagnitude(random(100, 200));
            vel.setMagnitude(random(5, 10));

            particles[i] = Particle(mass, pos.x, pos.y, vel.x, vel.y);
        }
    }

}