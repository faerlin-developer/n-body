#pragma once

#include "math/particle.h"

class PointData {
public:
    int index;

    PointData();

    explicit PointData(int index);
};

class NodeData {
public:
    float total_mass;
    float xCentreMass;
    float yCentreMass;
    Particle particle;

    NodeData();

    NodeData(float total_mass, float xCentreMass, float yCentreMass, Particle particle);
};