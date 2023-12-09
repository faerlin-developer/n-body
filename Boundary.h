#pragma once

namespace Boundary {

    class Rectangle {

    public:
        float xCenter;  // x-coordinate of the rectangle's center
        float yCenter;  // y-coordinate of the rectangle's center
        float w;        // half of the rectangle's width
        float h;        // half of the rectangle's height

        Rectangle() = default;

        Rectangle(float xCenter, float yCenter, float w, float h) : xCenter(xCenter), yCenter(yCenter), w(w), h(h) {}

        bool contains(float x, float y) const {
            auto containsX = this->xCenter - this->w <= x && x <= this->xCenter + this->w;
            auto containsY = this->yCenter - this->h <= y && y <= this->yCenter + this->h;

            return containsX && containsY;
        }

        bool intersects(Rectangle &other) const {
            auto xNoOverlap = this->xCenter + this->w < other.xCenter - other.w &&
                              other.xCenter + other.w < this->xCenter - this->w;
            auto yNoOverlap = this->yCenter + this->h < other.yCenter - other.h &&
                              other.yCenter + other.h < this->yCenter - this->h;
            auto noOverlap = xNoOverlap || yNoOverlap;
            return !noOverlap;
        }

    };

}