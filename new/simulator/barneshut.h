#pragma once

#include <cassert>
#include "math/particle.h"
#include "simulator.h"
#include "quadtree/quadtree.h"

class BarnesHut : public Simulator {

public:

    QuadTree<PointData, NodeData> *qTree;

    BarnesHut() : qTree(nullptr) {}

    void updateAcceleration(Particle *particles, int size, int start, int end) override {

        updateQuadTree(particles, size);

        updateNodeData(particles, qTree->root);

        auto leafs = qTree->leafs();

        for (int i = start; i <= end; i++) {
            updateAcceleration(particles, i, leafs);
        }
    }

    void updatePosition(Particle *particles, int size, int start, int end) override {

        for (int i = start; i <= end; i++) {
            particles[i].vel = particles[i].vel + particles[i].acc;
            particles[i].pos = particles[i].pos + particles[i].vel;
            particles[i].acc.set(0, 0);
        }
    }

    ~BarnesHut() {
        delete qTree;
    }

private:

    void updateQuadTree(Particle *particles, int size) {

        // Delete previous quad tree
        delete qTree;

        // Create new quad tree
        auto region = getConvexRegion(particles, size);
        qTree = new QuadTree<PointData, NodeData>(region, 8);

        // Populate quad tree with points
        for (int i = 0; i < size; i++) {
            auto pos = particles[i].pos;
            qTree->insert(Point(pos.x, pos.y, PointData(i)));
        }

    }

    void updateNodeData(Particle *particles, Node<PointData, NodeData> *node, bool leafOnly = false) {

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

    void updateAcceleration(Particle *particles, int i, std::vector<Node<PointData, NodeData> *> &leafs) {

        for (auto leaf: leafs) {

            assert(!leaf->points.empty());
            assert(leaf->data.total_mass != 0);

            auto distance = dist(particles[i].pos.x, particles[i].pos.y, leaf->data.xCentreMass,
                                 leaf->data.yCentreMass);

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

    static Region::Rectangle getConvexRegion(Particle *particles, int size) {

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

    static float dist(float x0, float y0, float x1, float y1) {
        return std::sqrt(std::pow(x1 - x0, 2) + std::pow(y1 - y0, 2));
    }

};