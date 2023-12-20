
#include "simulator/data.h"
#include "math/particle.h"

PointData::PointData() : index(0) {}

PointData::PointData(int index) : index(index) {}

NodeData::NodeData() : total_mass(0), xCentreMass(0), yCentreMass(0) {}

NodeData::NodeData(float total_mass, float xCentreMass, float yCentreMass, Particle particle) :
        total_mass(total_mass), xCentreMass(xCentreMass), yCentreMass(yCentreMass), particle(particle) {}