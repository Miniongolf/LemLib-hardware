#include "hardware/encoder/Rotation.hpp"

namespace lemlib {
Rotation::Rotation(int port)
    : rotation(port) {}

Rotation::Rotation(pros::Rotation rotation)
    : rotation(rotation) {}

void Rotation::calibrate() { rotation.reset(); }

bool Rotation::isCalibrated() { return rotation.is_installed(); }

bool Rotation::isCalibrating() { return false; }

bool Rotation::isConnected() { return rotation.is_installed(); }

Angle Rotation::getAbsoluteAngle() { return from_sDeg(rotation.get_angle() * 0.01); }

Angle Rotation::getRelativeAngle() { return from_sDeg(rotation.get_position() * 0.01); }

void Rotation::setAbsoluteAngle(Angle angle) { rotation.set_position(to_sDeg(angle) * 100); }

void Rotation::setRelativeAngle(Angle angle) { setAbsoluteAngle(angle); }

void Rotation::setReversed(bool reversed) { rotation.set_reversed(reversed); }

bool Rotation::isReversed() { return rotation.get_reversed(); }

} // namespace lemlib