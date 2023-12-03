#pragma once

namespace Boundary {

    class Rectangle {

    public:
        int xCenter;  // x-coordinate of the rectangle's center
        int yCenter;  // y-coordinate of the rectangle's center
        int w;        // half of the rectangle's width
        int h;        // half of the rectangle's height

        Rectangle() = default;

        Rectangle(int xCenter, int yCenter, int w, int h) : xCenter(xCenter), yCenter(yCenter), w(w), h(h) {}

        bool contains(int x, int y) {
            auto containsX = this->xCenter - this->w <= x && x <= this->xCenter + this->w;
            auto containsY = this->yCenter - this->h <= y && y <= this->yCenter + this->h;

            return containsX && containsY;
        }

        bool intersects(Rectangle &other) {
            auto xNoOverlap = this->xCenter + this->w < other.xCenter - other.w &&
                              other.xCenter + other.w < this->xCenter - this->w;
            auto yNoOverlap = this->yCenter + this->h < other.yCenter - other.h &&
                              other.yCenter + other.h < this->yCenter - this->h;
            auto noOverlap = xNoOverlap || yNoOverlap;
            return !noOverlap;
        }

    };

}