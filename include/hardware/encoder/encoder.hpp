#pragma once

#include "units/Angle.hpp"
#include "hardware/expected.hpp"

namespace lemlib {

/**
 * @brief enum class for encoder errors
 *
 */
enum class EncoderError { CALIBRATING, NOT_CALIBRATED, NOT_CONNECTED, UNKNOWN };

/**
 * @brief abstract class for encoders
 *
 * An encoder is a device that measures the angle of a rotating object. This class is abstract, and must be implemented
 * by a subclass.
 */
class Encoder {
    public:
        /**
         * @brief calibrates the encoder
         *
         * This calibrates resets the encoder. This function is non-blocking.
         */
        virtual rd::expected<void, EncoderError> calibrate() = 0;
        /**
         * @brief check if the encoder is calibrated
         *
         * @return rd::expected<bool, EncoderError>
         */
        virtual rd::expected<bool, EncoderError> isCalibrated() = 0;
        /**
         * @brief check if the encoder is calibrating
         *
         * @return rd::expected<bool, EncoderError>
         */
        virtual rd::expected<bool, EncoderError> isCalibrating() = 0;
        /**
         * @brief whether the encoder is connected
         *
         * @return rd::expected<bool, EncoderError>
         */
        virtual rd::expected<bool, EncoderError> isConnected() = 0;
        /**
         * @brief Get the absolute angle measured by the encoder
         *
         * The absolute angle measured by the encoder is the physical angle of the encoder. As such, it is bounded
         * between 0 and 360 degrees.
         *
         * @return rd::expected<Angle, EncoderError>
         */
        virtual rd::expected<Angle, EncoderError> getAbsoluteAngle() = 0;
        /**
         * @brief Get the relative angle measured by the encoder
         *
         * The relative angle measured by the encoder is the angle of the encoder relative to the last time the encoder
         * was reset. As such, it is unbounded.
         *
         * @return rd::expected<Angle, EncoderError>
         */
        virtual rd::expected<Angle, EncoderError> getRelativeAngle() = 0;
        /**
         * @brief Set the absolute angle of the encoder
         *
         * This function sets the absolute angle of the encoder. The absolute angle is the bounded angle of the encoder,
         * and may persist between power cycles if the hardware supports it. This function is non-blocking.
         *
         * @param angle
         * @return rd::expected<void, EncoderError>
         */
        virtual rd::expected<void, EncoderError> setAbsoluteAngle(Angle angle) = 0;
        /**
         * @brief Set the relative angle of the encoder
         *
         * This function sets the relative angle of the encoder. The relative angle is the number of rotations the
         * encoder has measured since the last reset. This function is non-blocking.
         *
         * @param angle
         * @return rd::expected<void, EncoderError>
         */
        virtual rd::expected<void, EncoderError> setRelativeAngle(Angle angle) = 0;
        /**
         * @brief Set the encoder to be reversed
         *
         * This function sets the encoder to be reversed. This function is non-blocking.
         *
         * @param reversed
         * @return rd::expected<void, EncoderError>
         */
        virtual rd::expected<void, EncoderError> setReversed(bool reversed) = 0;
        /**
         * @brief Check if the encoder is reversed
         *
         * @return rd::expected<bool, EncoderError>
         */
        virtual rd::expected<bool, EncoderError> isReversed() = 0;
        virtual ~Encoder() = default;
};
} // namespace lemlib