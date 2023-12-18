#pragma once

#include "quadtree/node.h"

namespace draw {

    void particles(Particle *particles, int size, sf::RenderWindow *window);

    void drawRegion(Node<PointData, NodeData> *node, sf::RenderWindow *window);

    void drawRectangle(Region::Rectangle &rect, sf::RenderWindow *window);

    void particles(Particle *particles, int size, sf::RenderWindow *window) {
        for (int i = 0; i < size; i++) {

            auto position = particles[i].pos;
            auto radius = particles[i].radius;

            sf::CircleShape circle(radius);
            circle.setPosition(position.x - radius, position.y - radius);
            circle.setFillColor(sf::Color{128, 128, 128});
            circle.setOutlineColor(sf::Color::White);
            circle.setOutlineThickness(2.0f);

            window->draw(circle);
        }
    }

    void drawRegion(Node<PointData, NodeData> *node, sf::RenderWindow *window) {

        drawRectangle(node->region, window);
        for (auto child: node->children) {
            drawRegion(child, window);
        }
    }

    void drawRectangle(Region::Rectangle &rect, sf::RenderWindow *window) {
        sf::RectangleShape rectangle(sf::Vector2f(2 * rect.w, 2 * rect.h));
        rectangle.setPosition(rect.xCenter - rect.w, rect.yCenter - rect.h);
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineColor(sf::Color::Red);
        rectangle.setOutlineThickness(1.0f);
        window->draw(rectangle);
    }
};