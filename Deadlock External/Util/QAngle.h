#pragma once
class QAngle {
public:
    float pitch, yaw, roll;

    // Constructors
    QAngle() : pitch(0), yaw(0), roll(0) {}
    QAngle(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll) {}

    // Operator overloads for basic math operations
    QAngle operator+(const QAngle& q) const {
        return QAngle(pitch + q.pitch, yaw + q.yaw, roll + q.roll);
    }

    QAngle operator-(const QAngle& q) const {
        return QAngle(pitch - q.pitch, yaw - q.yaw, roll - q.roll);
    }

    QAngle operator*(float scalar) const {
        return QAngle(pitch * scalar, yaw * scalar, roll * scalar);
    }

    QAngle operator/(float scalar) const {
        return QAngle(pitch / scalar, yaw / scalar, roll / scalar);
    }

    // Compound assignment operators
    QAngle& operator+=(const QAngle& q) {
        pitch += q.pitch;
        yaw += q.yaw;
        roll += q.roll;
        return *this;
    }

    QAngle& operator-=(const QAngle& q) {
        pitch -= q.pitch;
        yaw -= q.yaw;
        roll -= q.roll;
        return *this;
    }

    QAngle& operator*=(float scalar) {
        pitch *= scalar;
        yaw *= scalar;
        roll *= scalar;
        return *this;
    }

    QAngle& operator/=(float scalar) {
        pitch /= scalar;
        yaw /= scalar;
        roll /= scalar;
        return *this;
    }
};