
#include "simulator/barneshut.h"

BarnesHut::BarnesHut() : qTree(nullptr) {}

void BarnesHut::updateAcceleration(Particle *particles, int start, int end, int size) {

    updateQuadTree(particles, size);
    updateNodeData(particles, qTree->root, true);
    auto leafs = qTree->leafs();

    for (int i = start; i <= end; i++) {
        updateAcceleration(particles, i, leafs);
    }
}

void BarnesHut::updatePosition(Particle *particles, int start, int end, int size) {

    for (int i = start; i <= end; i++) {
        particles[i].vel = particles[i].vel + particles[i].acc;
        particles[i].pos = particles[i].pos + particles[i].vel;
        particles[i].acc.set(0, 0);
    }
}

void BarnesHut::draw(Particle *particles, int size, sf::RenderWindow *window, bool drawRegion) {

    draw::particles(particles, size, window);

    if (drawRegion) {
        draw::drawRegion(qTree->root, window);
    }
}

BarnesHut::~BarnesHut() {
    delete qTree;
}

void BarnesHut::updateQuadTree(Particle *particles, int size) {

    // Delete previous quad tree
    delete qTree;

    // Create new quad tree
    auto region = Particle::getConvexRegion(particles, size);
    qTree = new QuadTree<PointData, NodeData>(region, 8);

    // Populate quad tree with points
    for (int i = 0; i < size; i++) {
        auto pos = particles[i].pos;
        qTree->insert(Point(pos.x, pos.y, PointData(i)));
    }
}

void BarnesHut::updateNodeData(Particle *particles, Node<PointData, NodeData> *node, bool leafOnly) {

    node->data.total_mass = 0;
    node->data.xCentreMass = 0;
    node->data.yCentreMass = 0;

    // Process a leaf node containing points
    if (!node->points.empty()) {

        for (auto &point: node->points) {
            auto i = point.data.index;
            auto particle = particles[i];
            node->data.total_mass += particle.mass;
            node->data.xCentreMass += particle.mass * particle.pos.x;
            node->data.yCentreMass += particle.mass * particle.pos.y;
        }

        node->data.xCentreMass /= node->data.total_mass;
        node->data.yCentreMass /= node->data.total_mass;
        node->data.particle = Particle(node->data.total_mass,
                                       node->data.xCentreMass,
                                       node->data.yCentreMass, 0, 0);
    }

    // Process a node with children
    if (!node->children.empty()) {

        if (leafOnly) {

            for (auto child: node->children) {
                updateNodeData(particles, child);
            }

        } else {

            for (auto child: node->children) {
                updateNodeData(particles, child);
                node->data.total_mass += child->data.total_mass;
                node->data.xCentreMass += child->data.total_mass * child->data.xCentreMass;
                node->data.yCentreMass += child->data.total_mass * child->data.yCentreMass;
            }

            node->data.xCentreMass /= node->data.total_mass;
            node->data.yCentreMass /= node->data.total_mass;
        }
    }
}

void BarnesHut::updateAcceleration(Particle *particles, int i, std::vector<Node<PointData, NodeData> *> &leafs) {

    for (auto leaf: leafs) {

        assert(!leaf->points.empty());
        assert(leaf->data.total_mass != 0);

        auto distance = particles[i].dist(leaf->data.xCentreMass, leaf->data.yCentreMass);

        if (distance > 100) {

            particles[i].attractedBy(leaf->data.particle);

        } else {

            for (auto &point: leaf->points) {
                auto j = point.data.index;
                if (i != j) {
                    particles[i].attractedBy(particles[j]);
                }
            }

        }
    }
}