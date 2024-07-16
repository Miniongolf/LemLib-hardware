#pragma once

#include "units/Angle.hpp"

namespace lemlib {
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
         * This calibrates resets the encoder. This function is meant for encoders that may need to be calibrated in
         * order to function properly, not necessarily to tare the encoder. This function is non-blocking.
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     if (encoder->calibrate() == 0) {
         *         std::cout << "Encoder calibrating..." << std::endl;
         *     } else {
         *         std::cout << "Encoder calibration failed!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual int calibrate() = 0;
        /**
         * @brief check if the encoder is calibrated
         *
         * @return 0 if its not calibrated
         * @return 1 if it is calibrated
         * @return INT_MAX if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     encoder->calibrate();
         *     const int result = encoder->isCalibrated();
         *     if (result == 1) {
         *         std::cout << "Encoder is calibrated!" << std::endl;
         *     } else if (result == 0) {
         *         std::cout << "Encoder is not calibrated!" << std::endl;
         *     } else {
         *         std::cout << "Error checking if encoder is calibrated!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual int isCalibrated() = 0;
        /**
         * @brief check if the encoder is calibrating
         *
         * @return 0 if its not calibrating
         * @return 1 if it is calibrating
         * @return INT_MAX if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     encoder->calibrate();
         *     const int result = encoder->isCalibrating();
         *     if (result == 1) {
         *         std::cout << "Encoder is calibrating!" << std::endl;
         *     } else if (result == 0) {
         *         std::cout << "Encoder is not calibrating!" << std::endl;
         *     } else {
         *         std::cout << "Error checking if encoder is calibrating!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual int isCalibrating() = 0;
        /**
         * @brief whether the encoder is connected
         *
         * @return 0 if its not connected
         * @return 1 if it is connected
         * @return INT_MAX if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     const int result = encoder->isConnected();
         *     if (result == 1) {
         *         std::cout << "Encoder is connected!" << std::endl;
         *     } else if (result == 0) {
         *         std::cout << "Encoder is not connected!" << std::endl;
         *     } else {
         *         std::cout << "Error checking if encoder is connected!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual int isConnected() = 0;
        /**
         * @brief Get the absolute angle measured by the encoder
         *
         * The absolute angle measured by the encoder is the physical angle of the encoder. As such, it is bounded
         * between 0 and 360 degrees.
         *
         * @return Angle the absolute angle measured by the encoder
         * @return INFINITY if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     const Angle angle = encoder->getAbsoluteAngle();
         *     if (angle == INFINITY) {
         *         std::cout << "Error getting absolute angle!" << std::endl;
         *     } else {
         *         std::cout << "Absolute angle: " << angle.convert(deg) << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual Angle getAbsoluteAngle() = 0;
        /**
         * @brief Get the relative angle measured by the encoder
         *
         * The relative angle measured by the encoder is the angle of the encoder relative to the last time the encoder
         * was reset. As such, it is unbounded.
         *
         * @return Angle the relative angle measured by the encoder
         * @return INFINITY if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     const Angle angle = encoder->getRelativeAngle();
         *     if (angle == INFINITY) {
         *         std::cout << "Error getting relative angle!" << std::endl;
         *     } else {
         *         std::cout << "Relative angle: " << angle.convert(deg) << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual Angle getRelativeAngle() = 0;
        /**
         * @brief Set the absolute zero of the encoder to the current angle
         *
         * This function sets the absolute zero of the encoder to its current angle. The absolute angle is the bounded
         * angle of the encoder, and may persist between power cycles if the hardware supports it. This function is
         * non-blocking.
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     if (encoder->setAbsoluteZero() == 0) {
         *         std::cout << "Absolute zero set!" << std::endl;
         *         std::cout < "Absolute angle: " << encoder->getAbsoluteAngle().convert(deg) << std::endl; // outputs 0
         *     } else {
         *         std::cout << "Error setting absolute zero!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual int setAbsoluteZero() = 0;
        /**
         * @brief Set the relative angle of the encoder
         *
         * This function sets the relative angle of the encoder. The relative angle is the number of rotations the
         * encoder has measured since the last reset. This function is non-blocking.
         *
         * @param angle the relative angle to set the measured angle to
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     if (encoder->setRelativeAngle(0_stDeg) == 0) {
         *         std::cout << "Relative angle set!" << std::endl;
         *         std::cout < "Relative angle: " << encoder->getRelativeAngle().convert(deg) << std::endl; // outputs 0
         *     } else {
         *         std::cout << "Error setting relative angle!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual int setRelativeAngle(Angle angle) = 0;
        /**
         * @brief Set the encoder to be reversed
         *
         * This function sets the encoder to be reversed. This function is non-blocking.
         *
         * @param reversed whether the encoder should be reversed or not
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     if (encoder->setReversed(true) == 0) {
         *         std::cout << "Encoder reversed!" << std::endl;
         *     } else {
         *         std::cout << "Error reversing encoder!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual int setReversed(bool reversed) = 0;
        /**
         * @brief Check if the encoder is reversed
         *
         * @return 0 if its not reversed
         * @return 1 if it is reversed
         * @return INT_MAX if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Encoder* encoder = new ADIEncoder('A', 'B');
         *     const int result = encoder->isReversed();
         *     if (result == 1) {
         *         std::cout << "Encoder is reversed!" << std::endl;
         *     } else if (result == 0) {
         *         std::cout << "Encoder is not reversed!" << std::endl;
         *     } else {
         *         std::cout << "Error checking if encoder is reversed!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual int isReversed() = 0;
        virtual ~Encoder() = default;
};
} // namespace lemlib