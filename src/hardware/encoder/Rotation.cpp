#include "hardware/encoder/Rotation.hpp"
#include "hardware/util.hpp"
#include <limits.h>

namespace lemlib {
Rotation::Rotation(int port)
    : m_encoder(port) {}

Rotation::Rotation(std::uint8_t port, bool reversed)
    : m_encoder(int(port) * getSignFromBool(reversed)) // pros does not have a reversed flag for the pros::Rotation ctor
{}

Rotation::Rotation(pros::Rotation encoder)
    : m_encoder(encoder) {}

int Rotation::isConnected() { return m_encoder.is_installed(); }

Angle Rotation::getAngle() {
    const float angle = float(m_encoder.get_position()) / 100;
    // check for errors
    if (angle == INFINITY) return from_sDeg(INFINITY);
    return from_sDeg(angle);
}

int Rotation::setAngle(Angle angle) {
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