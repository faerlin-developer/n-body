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

        Rectangle(float xCenter, float yCenter, float w, float h)
                : xCenter(xCenter), yCenter(yCenter), w(w), h(h) {}

        /**
         * Returns true if the given x,y coordinate is within the boundary of this rectangle.
         *
         * @param x x-coordinate of a point.
         * @param y y-coordinate of a point.
         * @return
         */
        bool contains(float x, float y) const {
            auto containsX = this->xCenter - this->w <= x && x <= this->xCenter + this->w;
            auto containsY = this->yCenter - this->h <= y && y <= this->yCenter + this->h;
            return containsX && containsY;
        }
    };

}