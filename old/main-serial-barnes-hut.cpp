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

void drawCircle(Particle &particle, sf::RenderWindow &window) {
    auto r = particle.radius;
    sf::CircleShape circle(r);
    circle.setPosition(particle.pos.x - r, particle.pos.y - r);
    circle.setFillColor(sf::Color{128, 128, 128});
    circle.setOutlineColor(sf::Color::Green);
    circle.setOutlineThickness(2.0f);
    window.draw(circle);
}

void drawParticles(Particle *particles, int size, sf::RenderWindow &window) {

    for (int i = 0; i < size; i++) {
        auto r = particles[i].radius;
        sf::CircleShape circle(r);
        circle.setPosition(particles[i].pos.x - r, particles[i].pos.y - r);
        circle.setFillColor(sf::Color{128, 128, 128});
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(2.0f);
        window.draw(circle);
    }

}

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
        //auto mass = random(5, 15);
        auto mass = 10.0f;
        auto x = random(-1, 1);
        auto y = random(-1, 1);
        auto pos = Vector(x, y).unit();
        auto vel = pos;
        vel.rotate(M_PI / 2, Vector(0, 0));
        pos.setMagnitude(random(100, 300));
        vel.setMagnitude(random(5, 10));

        particles[i] = Particle(mass, pos.x + 400, pos.y + 400, vel.x, vel.y);
    }

    Simulator::BarnesHut simulator;

    /*
    window.clear();

    simulator.updateQuadTree(particles, N);
    simulator.updateCentreMass(particles, simulator.qTree->root);
    // draw(window, simulator.qTree->root);

    simulator.updateParticle(particles, 0, simulator.qTree->root, window);
    // drawCircle(particles[0], window);

    for (int i = 0; i < N; i++) {
        particles[i].vel = particles[i].vel + particles[i].acc;
        particles[i].pos = particles[i].pos + particles[i].vel;
        particles[i].acc.set(0, 0);
    }

    drawParticles(particles, N, window);

    window.display();
     */

    int frames = 0;
    sf::Clock clock_total;
    sf::Clock clock_update;

    window.setFramerateLimit(24);

    while (window.isOpen()) {
        clock_total.restart();

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        simulator.updateQuadTree(particles, N);
        simulator.updateCentreMass(particles, simulator.qTree->root);
        // draw(window, simulator.qTree->root);

        clock_update.restart();
        for (int i = 0; i < N; i++) {
            simulator.updateParticle(particles, i, simulator.qTree->root, window);
        }
        auto update_time = clock_update.getElapsedTime();
        // drawCircle(particles[0], window);

        for (int i = 0; i < N; i++) {
            particles[i].vel = particles[i].vel + particles[i].acc;
            particles[i].pos = particles[i].pos + particles[i].vel;
            particles[i].acc.set(0, 0);
        }

        drawParticles(particles, N, window);

        window.display();

        auto total_time = clock_total.getElapsedTime();

        if (frames % 24 == 0) {
            auto total_time_s = total_time.asSeconds();
            auto frame_rate = 1.f / total_time_s;
            std::cout << frame_rate << " " << update_time.asSeconds() << " " << total_time_s << std::endl;
        }

        frames++;
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

