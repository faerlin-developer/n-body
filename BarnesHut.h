#pragma once

#include "Particle.h"
#include "QuadTree2.h"

namespace Simulator {

    class NodeData {
        float total_mass;
        float centre_of_mass;
    };

    class BarnesHut {

    public:
        QuadTree<int, NodeData> *qTree;

        BarnesHut() : qTree(nullptr) {}

        void update(Particle *particles, int size, int start, int end) {

            updateQuadTree(particles, size);

        }

    private:

        Region::Rectangle getRegion(Particle *particles, int size) {

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

        void updateQuadTree(Particle *particles, int size) {

            // Delete previous quad tree
            delete qTree;

            // Create new quad tree
            auto region = getRegion(particles, size);
            qTree = new QuadTree<int, NodeData>(region, 1);

            // Populate quad tree with points
            for (int i = 0; i < size; i++) {
                auto pos = particles[i].pos;
                qTree->insert(Point(pos.x, pos.y, i));
            }

        }

        void updateCentreMass(Node<int, NodeData> *node) {
            // com_x = (x1*m1 + ... + xN*mN) / (m1 + ... + mN)
            // com_y = (y1*m1 + ... + yN*mN) / (m1 + ... + mN)

            

        }

        void updateParticles() {


        }


    };

}