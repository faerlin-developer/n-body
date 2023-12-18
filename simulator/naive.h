#pragma once

#include "math/particle.h"
#include "simulator.h"

class Naive : public Simulator {

public:

    void updateAcceleration(Particle *particles, int start, int end, int size) override {

        for (int i = start; i <= end; i++) {
            for (int j = 0; j < size; j++) {
                if (i != j) {
                    particles[i].attractedBy(particles[j]);
                }
            }
        }
    };

    void updatePosition(Particle *particles, int start, int end, int size) override {

        for (int i = start; i <= end; i++) {
            particles[i].vel = particles[i].vel + particles[i].acc;
            particles[i].pos = particles[i].pos + particles[i].vel;
            particles[i].acc.set(0, 0);
        }
    };

    void draw(Particle *particles, int size, sf::RenderWindow *window, bool drawRegion) {

        draw::particles(particles, size, window);

        if (drawRegion) {
            auto rectangle = Particle::getConvexRegion(particles, size);
            draw::drawRectangle(rectangle, window);
        }
    }

};