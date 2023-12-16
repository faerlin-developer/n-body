#pragma once

namespace draw {

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

};