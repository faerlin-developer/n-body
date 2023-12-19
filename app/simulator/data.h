#pragma once

class PointData {
public:
    int index;

    PointData() : index(0) {}

    explicit PointData(int index) : index(index) {}
};

class NodeData {
public:
    float total_mass;
    float xCentreMass;
    float yCentreMass;
    Particle particle;

    NodeData() : total_mass(0), xCentreMass(0), yCentreMass(0), particle(particle) {}

    NodeData(float total_mass, float xCentreMass, float yCentreMass, Particle particle) :
            total_mass(total_mass), xCentreMass(xCentreMass), yCentreMass(yCentreMass), particle(particle) {}
};