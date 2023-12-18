#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "math/region.h"
#include "point.h"
#include "node.h"

/**
 * A quadtree data structure.
 *
 * @tparam U User specific data in a Point object.
 * @tparam V User specific data in a Node object.
 */
template<class U, class V>
class QuadTree {

public:

    Node<U, V> *root;

    QuadTree() = delete;

    explicit QuadTree(Region::Rectangle region, int capacity) {
        root = new Node<U, V>(region, capacity);
    }

    /**
     * Inserts a point.
     */
    void insert(Point<U> point) {
        root->insert(point);
    }

    /**
     * Return the leaves of the quadtree.
     */
    std::vector<Node<U, V> *> leafs() {
        std::vector<Node<U, V> *> leafs;
        dfs(root, leafs);
        return leafs;
    }

    /**
     * Draw the boundaries of the regions of the quadtree.
     */
    void draw(sf::RenderWindow &window) {
        draw(window, root);
    }

    ~QuadTree() {
        delete root;
    }

private:

    void dfs(Node<U, V> *node, std::vector<Node<U, V> *> &leafs) {

        if (!node->points.empty()) {
            leafs.push_back(node);
            return;
        }

        for (auto child: node->children) {
            dfs(child, leafs);
        }

    }


private:

    /**
     * Draw the boundaries of the regions of the given node and its children.
     */
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