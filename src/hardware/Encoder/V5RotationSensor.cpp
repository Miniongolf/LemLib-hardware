#include "hardware/Encoder/V5RotationSensor.hpp"
#include <limits.h>

namespace lemlib {
V5RotationSensor::V5RotationSensor(pros::Rotation encoder)
    : m_encoder(encoder) {}

int V5RotationSensor::isConnected() { return m_encoder.is_installed(); }

Angle V5RotationSensor::getAngle() {
    const Angle angle = from_stDeg(double(m_encoder.get_position()) / 100);
    return angle + m_offset;
}

int V5RotationSensor::setAngle(Angle angle) {
    // requestedAngle = pos + offset
    // offset = requestedAngle - raw
    const Angle position = from_stDeg(double(m_encoder.get_position()) / 100);
    m_offset = angle - position;
    if (to_stRot(position) == INFINITY) return INT_MAX;
    return 0;
}
} // namespace lemlib