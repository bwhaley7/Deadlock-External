#pragma once
#include <cmath>

class Vector {
public:
    float x, y, z;

    // Constructors
    Vector() : x(0), y(0), z(0) {}
    Vector(float x, float y, float z) : x(x), y(y), z(z) {}

    // Operator overloads for basic math operations
    Vector operator+(const Vector& v) const {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator-(const Vector& v) const {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    Vector operator*(float scalar) const {
        return Vector(x * scalar, y * scalar, z * scalar);
    }

    Vector operator/(float scalar) const {
        return Vector(x / scalar, y / scalar, z / scalar);
    }

    // Compound assignment operators
    Vector& operator+=(const Vector& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector& operator-=(const Vector& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Other useful methods
    float Length() const {
        return sqrt(x * x + y * y + z * z);
    }

    float Dot(const Vector& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector Cross(const Vector& v) const {
        return Vector(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }
};