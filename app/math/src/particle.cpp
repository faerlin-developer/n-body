
#include <cmath>
#include "math/particle.h"
#include "math/region.h"

Particle::Particle() : Particle(1, 0, 0, 0, 0) {}

Particle::Particle(float mass, float x, float y, float vx, float vy)
        : mass(mass),
          radius(std::pow(mass, Particle::RADIUS_EXPONENT)),
          pos(x, y),
          vel(vx, vy),
          acc(0, 0) {}

Particle::Particle(Particle &other) {
    this->mass = other.mass;
    this->radius = other.radius;
    this->pos = other.pos;
    this->vel = other.vel;
    this->acc = other.acc;
}

Particle &Particle::operator=(const Particle &other) {

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
void Particle::attractedBy(Particle &other) {
    const float G = 0.1;
    auto acc = other.pos - this->pos;
    const auto magnitude = G * other.mass / std::min(std::max(10.f, acc.magnitudeSquared()), 1000.f);
    acc.setMagnitude(magnitude);
    this->acc = this->acc + acc;
}

float Particle::dist(float x, float y) const {
    return std::sqrt(std::pow(pos.x - x, 2) + std::pow(pos.y - y, 2));
}

Region::Rectangle Particle::getConvexRegion(Particle *particles, int size) {

    auto xMax = std::numeric_limits<float>::min();
    auto xMin = std::numeric_limits<float>::max();
    auto yMax = std::numeric_limits<float>::min();
    auto yMin = std::numeric_limits<float>::max();

    for (int i = 0; i < size; i++) {
        xMax = std::max(xMax, particles[i].pos.x);
        xMin = std::min(xMin, particles[i].pos.x);
        yMax = std::max(yMax, particles[i].pos.y);
        yMin = std::min(yMin, particles[i].pos.y);
    }

    auto width = (xMax - xMin) / 2.0f;
    auto height = (yMax - yMin) / 2.0f;
    auto xCenter = xMin + width;
    auto yCenter = yMin + height;

    return {xCenter, yCenter, width, height};
}