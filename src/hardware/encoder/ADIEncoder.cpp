#include "hardware/encoder/ADIEncoder.hpp"
#include <cmath>
#include <limits.h>

namespace lemlib {
ADIEncoder::ADIEncoder(char topPort, char bottomPort, bool reversed)
    : m_encoder(topPort, bottomPort, reversed),
      m_reversed(reversed) {}

ADIEncoder::ADIEncoder(pros::adi::ext_adi_port_tuple_t port, bool reversed)
    : m_encoder(port, reversed),
      m_reversed(reversed) {}

ADIEncoder::ADIEncoder(pros::adi::Encoder encoder, bool reversed)
    : m_encoder(encoder.get_port(), reversed), // the only way to set an encoder to be reversed is to create a new one
      m_reversed(reversed) {}

int ADIEncoder::calibrate() {
    // the ADIEncoder does not need to be calibrated, so we just check for errors and return 0 if there are none
    if (m_encoder.get_value() == INT_MAX) {
        errno = ENODEV;
        return INT_MAX;
    }
    // return 0 on success
    return 0;
}

int ADIEncoder::isCalibrated() {
    // the ADIEncoder does not need to be calibrated, so we just check for errors and return 1 if there are none
    if (m_encoder.get_value() == INT_MAX) {
        errno = ENODEV;
        return INT_MAX;
    }
    // return 1 to indicate that the encoder is "calibrated"
    return 1;
}

int ADIEncoder::isCalibrating() {
    // the ADIEncoder does not need to be calibrated, so we just check for errors and return 0 if there are none
    if (m_encoder.get_value() == INT_MAX) {
        errno = ENODEV;
        return INT_MAX;
    }
    // return 0 to indicate that the encoder is not calibrating
    return 0;
}

int ADIEncoder::isConnected() {
    // it's not possible to check if the ADIEncoder is connected, so we just return 1 to indicate that it is
    // we do run a simple test however to check if the ports are valid
    if (m_encoder.get_value() == INT_MAX) {
        errno = ENODEV;
        return INT_MAX;
    }
    // return 1 to indicate that the encoder is "connected"
    return 1;
}

Angle ADIEncoder::getAbsoluteAngle() {
    // the ADIEncoder does not support absolute angles, so we just return the relative angle
    return units::constrainAngle360(getRelativeAngle());
}

Angle ADIEncoder::getRelativeAngle() {
    const int raw = m_encoder.get_value();
    // check for errors
    if (raw == INT_MAX) {
        errno = ENODEV;
        return Angle(INFINITY);
    }
    // return the angle
    return from_sDeg(raw) + m_offset;
}

int ADIEncoder::setAbsoluteZero() {
    // the ADIEncoder does not support setting the absolute zero, so we set the relative angle instead
    return setRelativeAngle(0_stDeg);
}

int ADIEncoder::setRelativeAngle(Angle angle) {
    // the Vex SDK does not support setting the relative angle of an ADI encoder to a specific value
    // but we can overcome this limitation by resetting the relative angle to zero and saving an offset
    m_offset = angle;
    const int result = m_encoder.reset();
    // check for errors
    if (result == INT_MAX) {
        errno = ENODEV;
        return INT_MAX;
    }
    // return 0 on success
    return 0;
}

int ADIEncoder::setReversed(bool reversed) {
    // the only way to set an encoder to be reversed is to create a new one
    m_encoder = pros::adi::Encoder(m_encoder.get_port(), reversed);
    m_reversed = reversed;
    // check for errors
    if (m_encoder.get_value() == INT_MAX) {
        errno = ENODEV;
        return INT_MAX;
    }
    // return 0 on success
    return 0;
}

pros::adi::ext_adi_port_tuple_t ADIEncoder::getPort() const { return m_encoder.get_port(); }
} // namespace lemlib