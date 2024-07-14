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
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<void, EncoderError> calibrate() override;
        /**
         * @brief whether the encoder has been calibrated
         *
         * Since the V5 Rotation sensor does not need to be calibrated, this function just checks if the sensor is
         * connected or not
         *
         * @return tl::expected<bool, EncoderError>
         */
        tl::expected<bool, EncoderError> isCalibrated() override;
        /**
         * @brief whether the encoder is calibrating
         *
         * @return tl::expected<bool, EncoderError>
         */
        tl::expected<bool, EncoderError> isCalibrating() override;
        /**
         * @brief whether the encoder is connected
         *
         * @return tl::expected<bool, EncoderError>
         */
        tl::expected<bool, EncoderError> isConnected() override;
        /**
         * @brief Get the absolute angle of the encoder
         *
         * This function sets the absolute angle of the encoder. The absolute angle is the bounded angle of the encoder,
         * and it persists between power cycles.
         *
         * @return tl::expected<Angle, EncoderError>
         */
        tl::expected<Angle, EncoderError> getAbsoluteAngle() override;
        /**
         * @brief Set the relative angle of the encoder
         *
         * This function sets the relative angle of the encoder. The relative angle is the number of rotations the
         * encoder has measured since the last reset. This function is non-blocking.
         *
         * @param angle
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<Angle, EncoderError> getRelativeAngle() override;
        /**
         * @brief Set the absolute angle of the encoder
         *
         * The V5 Rotation sensor only supports setting the absolute angle to 0, so this function does that and ignores
         * the input angle
         *
         * @param angle
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<void, EncoderError> setAbsoluteAngle(Angle) override;
        /**
         * @brief Set the relative angle of the encoder
         *
         * This function sets the relative angle of the encoder. The relative angle is the number of rotations the
         * encoder has measured since the last reset. This function is non-blocking.
         *
         * @param angle
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<void, EncoderError> setRelativeAngle(Angle angle) override;
        /**
         * @brief Set whether the encoder is reversed or not
         *
         * this function is non-blocking
         *
         * @param reversed
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<void, EncoderError> setReversed(bool reversed) override;
        /**
         * @brief Get whether the encoder is reversed or not
         *
         * @return tl::expected<bool, EncoderError>
         */
        tl::expected<bool, EncoderError> isReversed() override;
        /**
         * @brief Get the port of the encoder
         *
         * @return tl::expected<int, EncoderError>
         */
        tl::expected<int, EncoderError> getPort();
    protected:
        pros::Rotation rotation;
};
} // namespace lemlib