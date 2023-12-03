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
#include "QuadTree.h"

void centerWindow(sf::RenderWindow *window);

float random(float min, float max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}

void update(Particle *particles, int i, Node<int> *node);

double dist(float x0, float y0, float x1, float y1);

int main(int argc, char *argv[]) {

    auto window = sf::RenderWindow(sf::VideoMode(800, 800), "SFML works!");
    centerWindow(&window);

    auto N = 1100;
    auto particles = new Particle[N];
    for (int i = 0; i < N; i++) {
        // auto mass = random(5, 15);
        auto mass = 10.0f;
        auto x = random(-1, 1);
        auto y = random(-1, 1);
        auto pos = Vector(x, y).unit();
        auto vel = pos;
        vel.rotate(M_PI / 2, Vector(0, 0));
        pos.setMagnitude(random(100, 200));
        vel.setMagnitude(random(5, 10));

        particles[i] = Particle(mass, pos.x + 400, pos.y + 400, vel.x, vel.y);
    }

    int frames = 0;
    sf::Clock clock_total;
    sf::Clock clock_update;
    sf::Clock clock_draw;

    window.setFramerateLimit(24);
    while (window.isOpen()) {
        clock_total.restart();

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        clock_update.restart();

        auto xMax = std::numeric_limits<float>::min();
        auto xMin = std::numeric_limits<float>::max();
        auto yMax = std::numeric_limits<float>::min();
        auto yMin = std::numeric_limits<float>::max();
        for (int i = 0; i < N; i++) {
            xMax = std::max(xMax, particles[i].pos.x);
            xMin = std::min(xMin, particles[i].pos.x);
            yMax = std::max(yMax, particles[i].pos.y);
            yMin = std::min(yMin, particles[i].pos.y);
        }

        int width = std::ceil((xMax - xMin) / 2.0f);
        int height = std::ceil((yMax - yMin) / 2.0f);
        int xCenter = std::ceil(xMin + width);
        int yCenter = std::ceil(yMin + height);

        auto boundary = Boundary::Rectangle(xCenter, yCenter, width, height);
        auto qTree = new QuadTree<int>(boundary, 8);

        for (int i = 0; i < N; i++) {
            qTree->insert(Point<int>(particles[i].pos.x, particles[i].pos.y, i));
        }

        auto root = qTree->root;
        for (int i = 0; i < N; i++) {
            update(particles, i, root);
        }

        for (int i = 0; i < N; i++) {
            particles[i].vel = particles[i].vel + particles[i].acc;
            particles[i].pos = particles[i].pos + particles[i].vel;
            particles[i].acc.set(0, 0);
        }
        auto update_time = clock_update.getElapsedTime();

        clock_draw.restart();

        window.clear();

        qTree->draw(window);

        for (int i = 0; i < N; i++) {
            auto r = particles[i].radius;
            sf::CircleShape circle(r);
            circle.setPosition(particles[i].pos.x - r, particles[i].pos.y - r);
            circle.setFillColor(sf::Color{128, 128, 128});
            circle.setOutlineColor(sf::Color::White);
            circle.setOutlineThickness(2.0f);
            window.draw(circle);
        }

        auto draw_time = clock_draw.getElapsedTime();
        window.display();
        auto total_time = clock_total.getElapsedTime();

        if (frames % 24 == 0) {

            auto update_time_s = update_time.asSeconds();
            auto draw_time_s = draw_time.asSeconds();
            auto total_time_s = total_time.asSeconds();
            auto frame_rate = 1.f / total_time_s;

            std::cout << frame_rate << " "
                      << update_time_s << " "
                      << draw_time_s << " "
                      << total_time_s << " "
                      << update_time_s / total_time_s << std::endl;
        }

        frames++;
    }

    return EXIT_SUCCESS;
}

void centerWindow(sf::RenderWindow *window) {
    auto desktop = sf::VideoMode::getDesktopMode();
    auto pos = sf::Vector2i(desktop.width / 2 - window->getSize().x / 2, desktop.height / 2 - window->getSize().y / 2);
    window->setPosition(pos);
}

double dist(float x0, float y0, float x1, float y1) {
    return std::pow(x1 - x0, 2) + std::pow(y1 - y0, 2);
}

void update(Particle *particles, int i, Node<int> *node) {

    // 25 ** 2 = 625
    auto d = dist(particles[i].pos.x, particles[i].pos.y, node->boundary.xCenter, node->boundary.yCenter);
    if (d < 625) {
        for (auto &point: node->points) {
            auto j = point.data;
            if (i != j) {
                particles[i].attractedBy(particles[j]);
            }

        }
    } else if (!node->points.empty()) {
        auto ghost = Particle(10 * node->points.size(),
                              node->boundary.xCenter, node->boundary.yCenter,
                              0, 0);
        particles[i].attractedBy(ghost);
    }

    for (auto child: node->children) {
        update(particles, i, child);
    }

}
