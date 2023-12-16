#pragma once

#include "math/particle.h"

class Naive : public Simulator {

public:

    void updateAcceleration(Particle *particles, int size, int start, int end) override {

        for (int i = start; i <= end; i++) {
            for (int j = start; j <= end; j++) {
                if (i != j) {
                    particles[i].attractedBy(particles[j]);
                }
            }
        }
    };

    void updatePosition(Particle *particles, int size, int start, int end) override {

        for (int i = start; i <= end; i++) {
            particles[i].vel = particles[i].vel + particles[i].acc;
            particles[i].pos = particles[i].pos + particles[i].vel;
            particles[i].acc.set(0, 0);
        }
    };

};