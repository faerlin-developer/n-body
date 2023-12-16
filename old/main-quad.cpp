#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include "Boundary.h"
#include "QuadTree.h"

void centerWindow(sf::RenderWindow *window);

int main(int argc, char *argv[]) {

    auto window = sf::RenderWindow(sf::VideoMode(800, 800), "SFML works!");

    sf::Vector2u size = window.getSize();
    sf::View view(sf::Vector2f(0, 0), sf::Vector2f(size.x, size.y));
    window.setView(view);

    centerWindow(&window);

    auto boundary = Boundary::Rectangle(0, 0, 350, 350);
    auto qTree = new QuadTree<int>(boundary, 4);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(-300, 300);

    auto points = std::vector<Point<int>>();
    for (int i = 0; i < 2000; i++) {
        auto x = dist(rng);
        auto y = dist(rng);
        points.push_back(Point<int>(x, y, i));
    }

    for (auto &point: points) {
        qTree->insert(point);
    }

    auto range = Boundary::Rectangle(0, 0, 100, 200);
    std::vector<int> qPoints;
    qTree->query(range, qPoints);

    window.setFramerateLimit(24);
    while (window.isOpen()) {

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        qTree->draw(window);

        for (auto &point: points) {
            auto r = 1.0f;
            sf::CircleShape circle(r);
            circle.setPosition(point.x - r, point.y - r);
            circle.setFillColor(sf::Color{128, 128, 128});
            circle.setOutlineColor(sf::Color::White);
            circle.setOutlineThickness(1.0f);
            window.draw(circle);
        }

        for (auto i: qPoints) {
            auto point = points[i];
            auto r = 1.0f;
            sf::CircleShape circle(r);
            circle.setPosition(point.x - r, point.y - r);
            circle.setFillColor(sf::Color{128, 128, 128});
            circle.setOutlineColor(sf::Color::Green);
            circle.setOutlineThickness(1.0f);
            window.draw(circle);
        }

        window.display();
    }


    return EXIT_SUCCESS;
}

void centerWindow(sf::RenderWindow *window) {
    auto desktop = sf::VideoMode::getDesktopMode();
    auto pos = sf::Vector2i(desktop.width / 2 - window->getSize().x / 2, desktop.height / 2 - window->getSize().y / 2);
    window->setPosition(pos);
}