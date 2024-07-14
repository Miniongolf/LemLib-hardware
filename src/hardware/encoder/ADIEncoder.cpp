#include "hardware/encoder/ADIEncoder.hpp"

namespace lemlib {
ADIEncoder::ADIEncoder(char topPort, char bottomPort, bool reversed)
    : encoder(topPort, bottomPort, reversed) {}

ADIEncoder::ADIEncoder(pros::adi::ext_adi_port_tuple_t port, bool reversed)
    : encoder(port, reversed) {}

ADIEncoder::ADIEncoder(pros::adi::Encoder ADIEncoder)
    : encoder(ADIEncoder) {}

tl::expected<void, EncoderError> ADIEncoder::calibrate() {
    // since the ADI Encoder does not need to be calibrated, we just check if it is connected
    // and we can get the value from it without any errors
    auto result = encoder.get_value();
    if (result == PROS_ERR) {
        if (errno == ENODEV) return tl::unexpected(EncoderError::UNKNOWN);
    }
    // return void if there are no errors
    return {};
}

tl::expected<bool, EncoderError> ADIEncoder::isCalibrated() {
    // since the ADI Encoder does not need to be calibrated, we just check if it is connected
    // and we can get the value from it without any errors
    auto result = encoder.get_value();
    if (result == PROS_ERR) {
        if (errno == ENODEV) return tl::unexpected(EncoderError::UNKNOWN);
    }
    // return true if there are no errors
    return true;
}

tl::expected<bool, EncoderError> ADIEncoder::isCalibrating() {
    // since the ADI Encoder does not need to be calibrated, we just check if it is connected
    // and we can get the value from it without any errors
    auto result = encoder.get_value();
    if (result == PROS_ERR) {
        if (errno == ENODEV) return tl::unexpected(EncoderError::UNKNOWN);
    }
    // return false if there are no errors
    return false;
}

tl::expected<bool, EncoderError> ADIEncoder::isConnected() {
    // since the ADI Encoder does not need to be calibrated, we just check if it is connected
    // and we can get the value from it without any errors
    auto result = encoder.get_value();
    if (result == PROS_ERR) {
        if (errno == ENODEV) return tl::unexpected(EncoderError::UNKNOWN);
    }
    return true;
}

tl::expected<Angle, EncoderError> ADIEncoder::getAbsoluteAngle() {
    // get the value from the encoder
    auto result = encoder.get_value();
    if (result == PROS_ERR) {
        if (errno == ENODEV) return tl::unexpected(EncoderError::UNKNOWN);
    }
    // return the value as an angle
    return from_sDeg(result);
}

tl::expected<Angle, EncoderError> ADIEncoder::getRelativeAngle() { return getAbsoluteAngle(); }

tl::expected<void, EncoderError> ADIEncoder::setAbsoluteAngle(Angle) {
    // since the ADIEncoder can't set its measured values to a specific value, we set the measured value to 0 as we
    // assume the user is calling this function to reset the encoder
    auto result = encoder.reset();
    if (result == PROS_ERR) {
        if (errno == ENODEV) return tl::unexpected(EncoderError::UNKNOWN);
    }
    // return void if there are no errors
    return {};
}

tl::expected<void, EncoderError> ADIEncoder::setRelativeAngle(Angle) { return setAbsoluteAngle(0_stDeg); }

tl::expected<void, EncoderError> ADIEncoder::setReversed(bool reversed) {
    // the only way to set the reversed value is to construct the encoder again
    encoder = pros::adi::Encoder(encoder.get_port(), reversed);
    return {};
}

tl::expected<bool, EncoderError> ADIEncoder::isReversed() {
    // we can't implement this function due to Vex SDK limitations
    return tl::unexpected(EncoderError::UNKNOWN);
}

tl::expected<pros::adi::ext_adi_port_tuple_t, EncoderError> ADIEncoder::getPort() {
    // there is no way for this function to return an error
    return encoder.get_port();
}
} // namespace lemlib