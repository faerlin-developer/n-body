#pragma once

#include <random>
#include "args/args.h"
#include "simulator/barneshut.h"
#include "simulator/naive.h"
#include "graphics/screen.h"
#include "math/particle.h"

namespace init {

    float random(float min, float max);

    void particles(Particle *particles, int size);

    void falsify(int *flag);

    Simulator *simulator(args::SimulatorType simulatorType);
}