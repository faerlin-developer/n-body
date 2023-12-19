
#include "math/state.h"

State::State() : State(0, 0, 0, 0) {}

State::State(float x, float y, float vx, float vy) : position(x, y), velocity(vx, vy) {}

State::State(State &other) {
    this->position = other.position;
    this->velocity = other.velocity;
}

State &State::operator=(const State &other) {

    if (this == &other) {
        return *this;
    }

    this->position = other.position;
    this->velocity = other.velocity;

    return *this;
}