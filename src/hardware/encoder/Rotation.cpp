#include "hardware/encoder/Rotation.hpp"
#include <limits.h>

namespace lemlib {
Rotation::Rotation(pros::Rotation encoder)
    : m_encoder(encoder) {}

int Rotation::isConnected() { return m_encoder.is_installed(); }

Angle Rotation::getAngle() {
    const float angle = float(m_encoder.get_position()) / 100;
    // check for errors
    if (angle == INFINITY) return from_stDeg(INFINITY);
    return from_stDeg(angle);
}

int Rotation::setAngle(Angle angle) {
    const int result = m_encoder.set_position(to_stDeg(angle) * 100);
    // check for errors
    if (result == INT_MAX) return INT_MAX;
    return 0;
}
} // namespace lemlib