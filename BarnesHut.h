#pragma once

#include "Particle.h"
#include "QuadTree2.h"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace Simulator {

    class NodeData {
    public:
        float total_mass;
        float xCentreMass;
        float yCentreMass;
    };

    class BarnesHut {

    public:
        QuadTree<int, NodeData> *qTree;

        BarnesHut() : qTree(nullptr) {}

        void update(Particle *particles, int size, int start, int end, sf::RenderWindow &window) {

            updateQuadTree(particles, size);
            updateCentreMass(particles, qTree->root);
            // updateParticles(particles, start, end, window);
        }

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
            qTree = new QuadTree<int, NodeData>(region, 8);

            // Populate quad tree with points
            for (int i = 0; i < size; i++) {
                auto pos = particles[i].pos;
                qTree->insert(Point(pos.x, pos.y, i));
            }

        }

        void updateCentreMass(Particle *particles, Node<int, NodeData> *node) {

            // com_x = (x1*m1 + ... + xN*mN) / (m1 + ... + mN)
            // com_y = (y1*m1 + ... + yN*mN) / (m1 + ... + mN)

            node->data.total_mass = 0;
            node->data.xCentreMass = 0;
            node->data.yCentreMass = 0;

            if (!node->points.empty()) {

                for (auto &point: node->points) {
                    auto index = point.data;
                    auto particle = particles[index];
                    node->data.total_mass += particle.mass;
                    node->data.xCentreMass += particle.mass * particle.pos.x;
                    node->data.yCentreMass += particle.mass * particle.pos.y;
                }

                node->data.xCentreMass /= node->data.total_mass;
                node->data.yCentreMass /= node->data.total_mass;

            } else if (!node->children.empty()) {

                for (auto child: node->children) {
                    updateCentreMass(particles, child);
                    node->data.total_mass += child->data.total_mass;
                    node->data.xCentreMass += child->data.total_mass * child->data.xCentreMass;
                    node->data.yCentreMass += child->data.total_mass * child->data.yCentreMass;
                }

                node->data.xCentreMass /= node->data.total_mass;
                node->data.yCentreMass /= node->data.total_mass;
            }
        }

        float dist(float x0, float y0, float x1, float y1) {
            return std::sqrt(std::pow(x1 - x0, 2) + std::pow(y1 - y0, 2));
        }

        void drawCircle(Particle &particle, sf::RenderWindow &window, sf::Color color) {
            auto r = particle.radius;
            sf::CircleShape circle(r);
            circle.setPosition(particle.pos.x - r, particle.pos.y - r);
            circle.setFillColor(sf::Color{128, 128, 128});
            circle.setOutlineColor(color);
            circle.setOutlineThickness(2.0f);
            window.draw(circle);
        }

        void drawRegion(sf::RenderWindow &window, Node<int, NodeData> *node) {
            sf::RectangleShape rectangle(sf::Vector2f(2 * node->region.w, 2 * node->region.h));
            rectangle.setPosition(node->region.xCenter - node->region.w, node->region.yCenter - node->region.h);
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineColor(sf::Color::Red);
            rectangle.setOutlineThickness(1.0f);
            window.draw(rectangle);
        }

    public:
        void updateParticle(Particle *particles, int i, Node<int, NodeData> *node, sf::RenderWindow &window) {

            if (node->points.empty() && node->children.empty()) {
                return;
            }

            float theta = 1.0;
            auto s = node->region.w + node->region.h;
            auto d = dist(particles[i].pos.x, particles[i].pos.y, node->data.xCentreMass, node->data.yCentreMass);
            // std::cout << particles[i].pos.x << " " << particles[i].pos.y << std::endl;
            if (d > 400) {

                auto ghost = Particle(node->data.total_mass, node->data.xCentreMass, node->data.yCentreMass, 0, 0);
                particles[i].attractedBy(ghost);

                // drawCircle(ghost, window, sf::Color::Red);

            } else {

                if (!node->points.empty()) {
                    for (auto &point: node->points) {
                        auto j = point.data;
                        if (i != j) {
                            particles[i].attractedBy(particles[j]);
                        }
                        // drawRegion(window, node);
                        // drawCircle(particles[j], window, sf::Color::White);
                    }

                } else {
                    for (Node<int, NodeData> *child: node->children) {
                        updateParticle(particles, i, child, window);
                    }
                }
            }
        }

        void updateParticles(Particle *particles, int start, int end, sf::RenderWindow &window) {

            for (int i = start; i <= end; i++) {
                updateParticle(particles, i, qTree->root, window);
            }

            for (int i = start; i <= end; i++) {
                particles[i].vel = particles[i].vel + particles[i].acc;
                particles[i].pos = particles[i].pos + particles[i].vel;
                particles[i].acc.set(0, 0);
            }

        }

    };

}