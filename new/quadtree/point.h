#pragma once

/**
 * A point object in a quadtree.
 *
 * @tparam U User data in a Point object.
 */
template<class U>
class Point {
public:
    float x;    // x coordinate
    float y;    // y coordinate
    U data;     // User specific data

    Point(float x, float y, U data) : x(x), y(y), data(data) {}
};