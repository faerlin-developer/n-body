#pragma once

#include "math/vector.h"

class State {
public:
    Vector position;
    Vector velocity;

    State();

    State(float x, float y, float vx, float vy);

    State(State &other);

    State &operator=(const State &other);
};
