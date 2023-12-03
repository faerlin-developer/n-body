#pragma once

#include <vector>

template<class U, class V>
class Node;

namespace Region {
    class Rectangle {
    public:
        float xCenter;  // x-coordinate of the rectangle's center
        float yCenter;  // y-coordinate of the rectangle's center
        float w;        // half of rectangle's width
        float h;        // half of rectangle's height.

        Rectangle(float xCenter, float yCenter, float w, float h) : xCenter(xCenter), yCenter(yCenter), w(w), h(h) {}

        bool contains(float x, float y) const {
            auto containsX = this->xCenter - this->w <= x && x <= this->xCenter + this->w;
            auto containsY = this->yCenter - this->h <= y && y <= this->yCenter + this->h;
            return containsX && containsY;
        }
    };
}

template<class U>
class Point {
public:
    float x;
    float y;
    U data;

    Point(float x, float y, U data) : x(x), y(y), data(data) {}
};

template<class U, class V>
class Node {
public:
    int capacity;
    Region::Rectangle region;
    std::vector<Point<U>> points;
    std::vector<Node<U, V> *> children;

    Node(Region::Rectangle region, int capacity) : region(region), capacity(capacity) {}

    bool insert(Point<U> point) {

        if (!region.contains(point.x, point.y)) {
            return false;
        }

        if (children.empty()) {

            points.push_back(point);
            if (points.size() > capacity) {
                subdivide();
            }

        } else {

            for (Node<U, V> *child: children) {
                if (child->insert(point)) {
                    continue;
                }
            }
        }

        return true;
    }

    void subdivide() {

        auto x = region.xCenter;
        auto y = region.yCenter;
        auto w = region.w / 2;
        auto h = region.h / 2;

        auto northeast = new Node<U, V>(Region::Rectangle(x + w, y - h, w, h), capacity);
        auto northwest = new Node<U, V>(Region::Rectangle(x - w, y - h, w, h), capacity);
        auto southeast = new Node<U, V>(Region::Rectangle(x + w, y + h, w, h), capacity);
        auto southwest = new Node<U, V>(Region::Rectangle(x - w, y + h, w, h), capacity);

        children.push_back(northeast);
        children.push_back(northwest);
        children.push_back(southeast);
        children.push_back(southwest);

        for (auto p: points) {
            for (Node<U, V> *child: children) {
                if (child->insert(p)) {
                    break;
                }
            }
        }

        points.clear();
    }

    ~Node() {
        for (auto child: this->children)
            delete child;
    }
};

template<class U, class V>
class QuadTree {
public:

    Node<U, V> *root;

    QuadTree() = delete;

    explicit QuadTree(Region::Rectangle region, int capacity) {
        root = new Node<U, V>(region, capacity);
    }

    void insert(Point<U> point) {
        root->insert(point);
    }

    void draw(sf::RenderWindow &window) {
        draw(window, root);
    }

    ~QuadTree() {
        delete root;
    }

private:
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

};