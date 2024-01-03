#pragma once

namespace Region {

    /**
     * A rectangular region.
     */
    class Rectangle {

    public:

        float xCenter;  // x-coordinate of the rectangle's center.
        float yCenter;  // y-coordinate of the rectangle's center.
        float w;        // half of rectangle's width.
        float h;        // half of rectangle's height.

        Rectangle(float xCenter, float yCenter, float w, float h);

        bool contains(float x, float y) const;
    };
}