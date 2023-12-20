#pragma once

#include <cassert>
#include "math/particle.h"
#include "simulator/simulator.h"
#include "simulator/data.h"
#include "quadtree/quadtree.h"
#include "graphics/draw.h"

class BarnesHut : public Simulator {

public:

    QuadTree<PointData, NodeData> *qTree;

    BarnesHut();

    void updateAcceleration(Particle *particles, int start, int end, int size) override;

    void updatePosition(Particle *particles, int start, int end, int size) override;

    void draw(Particle *particles, int size, sf::RenderWindow *window, bool drawRegion) override;

    ~BarnesHut() override;

private:

    void updateQuadTree(Particle *particles, int size);

    void updateNodeData(Particle *particles, Node<PointData, NodeData> *node, bool leafOnly = false);

    static void updateAcceleration(Particle *particles, int i, std::vector<Node<PointData, NodeData> *> &leafs);
};