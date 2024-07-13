#pragma once

#include "units/Angle.hpp"

namespace lemlib {
class Encoder {
    public:
        /**
         * @brief reset the encoder
         *
         * This function resets the encoder. This function is non-blocking.
         */
        virtual void calibrate() = 0;
        /**
         * @brief check if the encoder is calibrated
         *
         * @return true the encoder is calibrated
         * @return false the encoder is not calibrated
         */
        virtual bool isCalibrated() = 0;
        /**
         * @brief check if the encoder is calibrating
         *
         * @return true the encoder is calibrating
         * @return false the encoder is not calibrating
         */
        virtual bool isCalibrating() = 0;
        /**
         * @brief whether the encoder is connected
         *
         * @return true the encoder is connect
         * @return false the encoder is not connected
         */
        virtual bool isConnected() = 0;
        /**
         * @brief Get the absolute angle measured by the encoder
         *
         * The absolute angle measured by the encoder is the physical angle of the encoder. As such, it is bounded
         * between 0 and 360 degrees.
         *
         * @return Angle
         */
        virtual Angle getAbsoluteAngle() = 0;
        /**
         * @brief Get the relative angle measured by the encoder
         *
         * The relative angle measured by the encoder is the angle of the encoder relative to the last time the encoder
         * was reset. As such, it is unbounded.
         *
         * @return Angle
         */
        virtual Angle getRelativeAngle() = 0;
        /**
         * @brief Set the absolute angle of the encoder
         *
         * This function sets the absolute angle of the encoder. The absolute angle is the bounded angle of the encoder,
         * and may persist between power cycles if the hardware supports it. This function is non-blocking.
         *
         * @param angle
         */
        virtual void setAbsoluteAngle(Angle angle) = 0;
        /**
         * @brief Set the relative angle of the encoder
         *
         * This function sets the relative angle of the encoder. The relative angle is the number of rotations the
         * encoder has measured since the last reset. This function is non-blocking.
         *
         * @param angle
         */
        virtual void setRelativeAngle(Angle angle) = 0;
        /**
         * @brief Set the encoder to be reversed
         *
         * This function sets the encoder to be reversed. This function is non-blocking.
         *
         * @param reversed
         */
        virtual void setReversed(bool reversed) = 0;
        /**
         * @brief Check if the encoder is reversed
         *
         * @return true the encoder is reversed
         * @return false the encoder is not reversed
         */
        virtual bool isReversed() = 0;
        virtual ~Encoder() = default;
};
} // namespace lemlib