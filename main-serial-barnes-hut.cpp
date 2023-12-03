#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <iostream>
#include <limits>
#include "Particle.h"
#include "State.h"
#include "Vector.h"
#include "Particle.h"
#include "Boundary.h"
#include "BarnesHut.h"

template<class U, class V>
void draw(sf::RenderWindow &window, Node<U, V> *node);

void centerWindow(sf::RenderWindow *window) {
    auto desktop = sf::VideoMode::getDesktopMode();
    auto pos = sf::Vector2i(desktop.width / 2 - window->getSize().x / 2, desktop.height / 2 - window->getSize().y / 2);
    window->setPosition(pos);
}

float random(float min, float max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}

int main(int argc, char *argv[]) {

    auto window = sf::RenderWindow(sf::VideoMode(800, 800), "SFML works!");
    centerWindow(&window);

    auto N = 1100;
    auto particles = new Particle[N];
    for (int i = 0; i < N; i++) {
        auto mass = random(5, 15);
        // auto mass = 10.0f;
        auto x = random(-1, 1);
        auto y = random(-1, 1);
        auto pos = Vector(x, y).unit();
        auto vel = pos;
        vel.rotate(M_PI / 2, Vector(0, 0));
        pos.setMagnitude(random(100, 200));
        vel.setMagnitude(random(5, 10));

        particles[i] = Particle(mass, pos.x + 400, pos.y + 400, vel.x, vel.y);
    }

    Simulator::BarnesHut simulator;

    window.setFramerateLimit(24);
    while (window.isOpen()) {

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        simulator.update(particles, N, 0, N);
        draw(window, simulator.qTree->root);


        window.display();
    }

    window.close();

    return EXIT_SUCCESS;
}

template<class U, class V>
void draw(sf::RenderWindow &window, Node<U, V> *node) {

    sf::RectangleShape rectangle(sf::Vector2f(2 * node->region.w, 2 * node->region.h));
    rectangle.setPosition(node->region.xCenter - node->region.w, node->region.yCenter - node->region.h);
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(1.0f);
    window.draw(rectangle);

    for (auto child: node->children) {
        draw(window, child);
    }

}

