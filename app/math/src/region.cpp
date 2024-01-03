
#include "math/region.h"

Region::Rectangle::Rectangle(float xCenter, float yCenter, float w, float h)
        : xCenter(xCenter), yCenter(yCenter), w(w), h(h) {}

bool Region::Rectangle::contains(float x, float y) const {
    auto containsX = this->xCenter - this->w <= x && x <= this->xCenter + this->w;
    auto containsY = this->yCenter - this->h <= y && y <= this->yCenter + this->h;
    return containsX && containsY;
}