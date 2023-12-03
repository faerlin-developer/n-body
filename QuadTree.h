#pragma once

#include <vector>
#include <map>
#include <memory>
#include "Boundary.h"
#include "Particle.h"

template<class T>
class Point {
public:
    int x;
    int y;
    T data;

    Point() = delete;

    Point(int x, int y, T data) : x(x), y(y), data(data) {}
};

template<class T>
class Node {
public:
    int capacity;
    Boundary::Rectangle boundary;
    std::vector<Point<T>> points;
    std::vector<Node *> children;

    Node(Boundary::Rectangle boundary, int capacity) : boundary(boundary), capacity(capacity) {}

    bool insert(Point<T> point) {

        if (!boundary.contains(point.x, point.y)) {
            return false;
        }

        if (points.size() < capacity) {
            points.push_back(point);
            return true;
        }

        if (children.empty()) {
            subdivide();
        }

        for (auto child: children) {
            auto success = child->insert(point);
            if (success) {
                return true;
            }
        }

        return false;
    }

    void subdivide() {

        auto x = this->boundary.xCenter;
        auto y = this->boundary.yCenter;
        auto w = this->boundary.w / 2;
        auto h = this->boundary.h / 2;

        auto northeast = new Node<T>(Boundary::Rectangle(x + w, y - h, w, h), capacity);
        auto northwest = new Node<T>(Boundary::Rectangle(x - w, y - h, w, h), capacity);
        auto southeast = new Node<T>(Boundary::Rectangle(x + w, y + h, w, h), capacity);
        auto southwest = new Node<T>(Boundary::Rectangle(x - w, y + h, w, h), capacity);

        this->children.push_back(northeast);
        this->children.push_back(northwest);
        this->children.push_back(southeast);
        this->children.push_back(southwest);
    }

    ~Node() {
        for (auto child: this->children)
            delete child;
    }
};

template<class T>
class QuadTree {
public:
    int capacity;
    Node<T> *root;

    QuadTree() = delete;

    QuadTree(Boundary::Rectangle boundary, int capacity) : capacity(capacity) {
        root = new Node<T>(boundary, capacity);
    }

    bool insert(Point<T> point) {
        return this->root->insert(point);
    }

    void draw(sf::RenderWindow &window) {
        draw(window, root);
    }

    void query(Boundary::Rectangle &range, std::vector<T> &result) {
        query(range, result, root);
    }

    ~QuadTree() {
        delete this->root;
    }

private:
    void query(Boundary::Rectangle &range, std::vector<T> &result, Node<T> *node) {

        if (!range.intersects(node->boundary)) {
            return;
        }

        for (auto point: node->points) {
            if (range.contains(point.x, point.y)) {
                result.push_back(point.data);
            }
        }

        for (auto child: node->children) {
            query(range, result, child);
        }

    }

    void draw(sf::RenderWindow &window, Node<T> *node) {

        sf::RectangleShape rectangle(sf::Vector2f(2 * node->boundary.w, 2 * node->boundary.h));
        rectangle.setPosition(node->boundary.xCenter - node->boundary.w, node->boundary.yCenter - node->boundary.h);
        rectangle.setFillColor(sf::Color::Black);
        rectangle.setOutlineColor(sf::Color::White);
        rectangle.setOutlineThickness(1.0f);
        window.draw(rectangle);

        for (auto child: node->children) {
            draw(window, child);
        }

    }


};