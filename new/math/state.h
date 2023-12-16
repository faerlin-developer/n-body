#pragma once

#include "vector.h"

class State {
public:
    Vector position;
    Vector velocity;

    State() : State(0, 0, 0, 0) {}

    State(float x, float y, float vx, float vy) : position(x, y), velocity(vx, vy) {}

    State(State &other) {
        this->position = other.position;
        this->velocity = other.velocity;
    }

    State &operator=(const State &other) {

        if (this == &other) {
            return *this;
        }

        this->position = other.position;
        this->velocity = other.velocity;

        return *this;
    }
};
