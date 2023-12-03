#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include "QuadTree2.h"

class NodeData {
    float total_mass;
    float centre_of_mass;
};

void centerWindow(sf::RenderWindow *window);

int main(int argc, char *argv[]) {

    auto window = sf::RenderWindow(sf::VideoMode(800, 800), "SFML works!");

    sf::Vector2u size = window.getSize();
    sf::View view(sf::Vector2f(0, 0), sf::Vector2f(size.x, size.y));
    window.setView(view);

    centerWindow(&window);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<double> dist(0, 100);

    auto N = 1000;
    auto points = std::vector<Point<int>>();
    for (int i = 0; i < N; i++) {
        auto x = dist(mt);
        auto y = dist(mt);
        points.push_back(Point<int>(x, y, i));
    }

    auto xMax = std::numeric_limits<float>::min();
    auto xMin = std::numeric_limits<float>::max();
    auto yMax = std::numeric_limits<float>::min();
    auto yMin = std::numeric_limits<float>::max();
    for (int i = 0; i < N; i++) {
        xMax = std::max(xMax, points[i].x);
        xMin = std::min(xMin, points[i].x);
        yMax = std::max(yMax, points[i].y);
        yMin = std::min(yMin, points[i].y);
    }

    int width = std::ceil((xMax - xMin) / 2.0f);
    int height = std::ceil((yMax - yMin) / 2.0f);
    int xCenter = std::ceil(xMin + width);
    int yCenter = std::ceil(yMin + height);

    auto region = Region::Rectangle(xCenter, yCenter, width, height);
    auto qTree = QuadTree<int, NodeData>(region, 1);

    for (auto point: points) {
        qTree.insert(point);
    }

    window.setFramerateLimit(24);
    while (window.isOpen()) {

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        qTree.draw(window);

        for (auto &point: points) {
            auto r = 1.0f;
            sf::CircleShape circle(r);
            circle.setPosition(point.x - r, point.y - r);
            circle.setFillColor(sf::Color{128, 128, 128});
            circle.setOutlineColor(sf::Color::White);
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