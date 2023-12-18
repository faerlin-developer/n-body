#pragma once

#include <vector>
#include "math/region.h"
#include "point.h"

/**
 * A node representing one quadrant in the quadtree.
 *
 * @tparam U User data in a Point object.
 * @tparam V User data in a Node object.
 */
template<class U, class V>
class Node {

public:
    int capacity;                           // The maximum number of points in this node
    Region::Rectangle region;               // The spatial boundary of the quadrant represented by this node
    std::vector<Point<U>> points;           // The number of points in this node
    std::vector<Node<U, V> *> children;     // The children of this node
    V data;                                 // User specific data

    Node(Region::Rectangle region, int capacity) : region(region), capacity(capacity) {}

    /**
     * Insert a point to the quadtree.
     *
     * @param point The new point being inserted to the quadtree.
     * @return true when the provided point is inserted successfully; otherwise, return false.
     */
    bool insert(Point<U> point) {

        if (!region.contains(point.x, point.y)) {
            return false;
        }

        if (children.empty()) {

            points.push_back(point);

            if (points.size() > capacity) {

                // Initialize the children of this node
                subdivide();

                // Move the points of this node to its children
                for (auto p: points) {
                    for (Node<U, V> *child: children) {
                        if (child->insert(p)) {
                            break;
                        }
                    }
                }

                points.clear();
            }

        } else {

            // Pass the new point to the children of this node
            for (Node<U, V> *child: children) {
                if (child->insert(point)) {
                    break;
                }
            }
        }

        return true;
    }

    /**
     * Create the four sub-quadrant of this node by initializing the children of this node.
     */
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
    }

    ~Node() {
        for (auto child: this->children)
            delete child;
    }
};