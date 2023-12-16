#pragma once

#include <cmath>

class Vector {
public:
    float x;
    float y;

    Vector() : Vector(0, 0) {}

    Vector(float x, float y) : x(x), y(y) {}

    Vector(Vector &other) {
        this->x = other.x;
        this->y = other.y;
    }

    Vector &operator=(const Vector &other) {

        if (this == &other) {
            return *this;
        }

        this->x = other.x;
        this->y = other.y;

        return *this;
    }

    Vector operator+(const Vector &other) const {
        return {this->x + other.x, this->y + other.y};
    }

    Vector operator-(const Vector &other) const {
        return {this->x - other.x, this->y - other.y};
    }

    float magnitudeSquared() const {
        return std::pow(this->x, 2) + std::pow(this->y, 2);
    }

    float magnitude() const {
        return std::sqrt(magnitudeSquared());
    }

    Vector unit() const {
        auto mag = magnitude();
        return {this->x / mag, this->y / mag};
    }

    void normalize() {
        auto mag = magnitude();
        this->x = this->x / mag;
        this->y = this->y / mag;
    }

    Vector &setMagnitude(float magnitude) {
        normalize();
        this->x = this->x * magnitude;
        this->y = this->y * magnitude;
        return *this;
    }

    void set(float x, float y) {
        this->x = x;
        this->y = y;
    }

    // Rotate the vector clock-wise by angle in radians.
    void rotate(float angle, const Vector &aroundPoint) {
        float cosine = std::cos(angle);
        float sine = std::sin(angle);

        auto thisVec = Vector(this->x, this->y);
        thisVec = thisVec - aroundPoint;

        float xRot = thisVec.x * cosine - thisVec.y * sine;
        float yRot = thisVec.x * sine + thisVec.y * cosine;

        auto rot = Vector(xRot, yRot);

        *this = rot + aroundPoint;
    }

};




