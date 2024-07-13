#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "pros/rotation.hpp"

namespace lemlib {
class Rotation : public Encoder {
    public:
        /**
         * @brief Construct a new Rotation object
         *
         * @param port the signed port of the rotation sensor. If the port is negative, the sensor will be reversed.
         */
        Rotation(int port);
        /**
         * @brief Construct a new Rotation object
         *
         * @param rotation the pros::Rotation object to use
         */
        Rotation(pros::Rotation rotation);
        /**
         * @brief calibrate the encoder
         *
         * This function calibrates the encoder. The V5 Rotation sensor does not actually need to be calibrated, so this
         * function does nothing
         *
         * @return rd::expected<void, EncoderError>
         */
        rd::expected<void, EncoderError> calibrate() override;
        /**
         * @brief whether the encoder has been calibrated
         *
         * Since the V5 Rotation sensor does not need to be calibrated, this function just checks if the sensor is
         * connected or not
         *
         * @return rd::expected<bool, EncoderError>
         */
        rd::expected<bool, EncoderError> isCalibrated() override;
        /**
         * @brief whether the encoder is calibrating
         *
         * @return rd::expected<bool, EncoderError>
         */
        rd::expected<bool, EncoderError> isCalibrating() override;
        /**
         * @brief whether the encoder is connected
         *
         * @return rd::expected<bool, EncoderError>
         */
        rd::expected<bool, EncoderError> isConnected() override;
        /**
         * @brief Get the absolute angle of the encoder
         *
         * This function sets the absolute angle of the encoder. The absolute angle is the bounded angle of the encoder,
         * and it persists between power cycles.
         *
         * @return rd::expected<Angle, EncoderError>
         */
        rd::expected<Angle, EncoderError> getAbsoluteAngle() override;
        /**
         * @brief Set the relative angle of the encoder
         *
         * This function sets the relative angle of the encoder. The relative angle is the number of rotations the
         * encoder has measured since the last reset. This function is non-blocking.
         *
         * @param angle
         * @return rd::expected<void, EncoderError>
         */
        rd::expected<Angle, EncoderError> getRelativeAngle() override;
        /**
         * @brief Set the absolute angle of the encoder
         *
         * The V5 Rotation sensor only supports setting the absolute angle to 0, so this function ignores the given
         * angle and sets the absolute angle to 0. This function is non-blocking.
         *
         * @param angle
         * @return rd::expected<void, EncoderError>
         */
        rd::expected<void, EncoderError> setAbsoluteAngle(Angle) override;
        /**
         * @brief Set the relative angle of the encoder
         *
         * This function sets the relative angle of the encoder. The relative angle is the number of rotations the
         * encoder has measured since the last reset. This function is non-blocking.
         *
         * @param angle
         * @return rd::expected<void, EncoderError>
         */
        rd::expected<void, EncoderError> setRelativeAngle(Angle angle) override;
        /**
         * @brief Set whether the encoder is reversed or not
         *
         * this function is non-blocking
         *
         * @param reversed
         * @return rd::expected<void, EncoderError>
         */
        rd::expected<void, EncoderError> setReversed(bool reversed) override;
        /**
         * @brief Get whether the encoder is reversed or not
         *
         * @return rd::expected<bool, EncoderError>
         */
        rd::expected<bool, EncoderError> isReversed() override;
        /**
         * @brief Get the port of the encoder
         *
         * @return rd::expected<int, EncoderError>
         */
        rd::expected<int, EncoderError> getPort();
    protected:
        pros::Rotation rotation;
};
} // namespace lemlib