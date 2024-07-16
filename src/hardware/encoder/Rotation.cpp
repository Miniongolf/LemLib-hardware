#include "hardware/encoder/Rotation.hpp"
#include <limits.h>

int getSignFromBool(bool x) { return x ? -1 : 1; }

namespace lemlib {
Rotation::Rotation(int port)
    : m_encoder(port) {}

Rotation::Rotation(std::uint8_t port, bool reversed)
    : m_encoder(int(port) * getSignFromBool(reversed)) // pros does not have a reversed flag for the pros::Rotation ctor
{}

Rotation::Rotation(pros::Rotation encoder)
    : m_encoder(encoder) {}

int Rotation::calibrate() {
    // the v5 rotation sensor does not need to be calibrated, so we just check for errors and return 0 if there are none
    if (m_encoder.get_position() == INT_MAX) return INT_MAX;
    return 0;
}

int Rotation::isCalibrated() {
    // the v5 rotation sensor does not need to be calibrated, so we just check for errors and return 1 if there are none
    if (m_encoder.get_position() == INT_MAX) return INT_MAX;
    return 1;
}

int Rotation::isCalibrating() {
    // the v5 rotation sensor does not need to be calibrated, so we just check for errors and return 0 if there are none
    if (m_encoder.get_position() == INT_MAX) return INT_MAX;
    return 0;
}

int Rotation::isConnected() { return m_encoder.is_installed(); }

Angle Rotation::getAbsoluteAngle() {
    const float angle = float(m_encoder.get_angle()) / 100;
    // check for errors
    if (angle == INFINITY) return from_sDeg(INFINITY);
    return from_sDeg(angle);
}

Angle Rotation::getRelativeAngle() {
    const float angle = float(m_encoder.get_position()) / 100;
    // check for errors
    if (angle == INFINITY) return from_sDeg(INFINITY);
    return from_sDeg(angle);
}

int Rotation::setAbsoluteZero() {
    // there is currently no way to set the absolute zero position of the v5 rotation sensor, so we set its relative
    // position to 0 instead
    const int result = m_encoder.reset();
    // check for errors
    if (result == INT_MAX) return INT_MAX;
    return 0;
}

int Rotation::setRelativeAngle(Angle angle) {
    const int result = m_encoder.set_position(to_sDeg(angle) * 100);
    // check for errors
    if (result == INT_MAX) return INT_MAX;
    return 0;
}

int Rotation::setReversed(bool reversed) {
    const int result = m_encoder.set_reversed(reversed);
    // check for errors
    if (result == INT_MAX) return INT_MAX;
    return 0;
}

int Rotation::isReversed() {
    const int result = m_encoder.get_reversed();
    // check for errors
    if (result == INT_MAX) return INT_MAX;
    return result;
}

int Rotation::getPort() const { return m_encoder.get_port(); }
} // namespace lemlib