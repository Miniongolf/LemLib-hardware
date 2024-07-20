#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "pros/rotation.hpp"

namespace lemlib {
/**
 * @brief Encoder implementation for the V5 Rotation sensor
 *
 */
class Rotation : public Encoder {
    public:
        /**
         * @brief Construct a new Rotation encoder object
         *
         * @param encoder the pros::Rotation object to use
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Rotation pros_encoder(1);
         *     // construct by passing the pros::Rotation object
         *     lemlib::Rotation encoder(pros_encoder);
         *     // construct by implicitly converting the pros::Rotation object
         *     lemlib::Rotation encoder = pros_encoder;
         * }
         * @endcode
         */
        Rotation(pros::Rotation encoder);
        /**
         * @brief whether the encoder is connected
         *
         * @return 0 if its not connected
         * @return 1 if it is connected
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::Rotation encoder = pros::Rotation(1);
         *     const int result = encoder.isConnected();
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
        int isConnected() override;
        /**
         * @brief Get the relative angle measured by the encoder
         *
         * The relative angle measured by the encoder is the angle of the encoder relative to the last time the encoder
         * was reset. As such, it is unbounded.
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENXIO: the port is not within the range of valid ports (1-21)
         * ENODEV: the port cannot be configured as an V5 Rotation sensor
         *
         * @return Angle the relative angle measured by the encoder
         * @return INFINITY if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::Rotation encoder = pros::Rotation(1);
         *     const Angle angle = encoder.getAngle();
         *     if (angle == INFINITY) {
         *         std::cout << "Error getting relative angle!" << std::endl;
         *     } else {
         *         std::cout << "Relative angle: " << angle.convert(deg) << std::endl;
         *     }
         * }
         * @endcode
         */
        Angle getAngle() override;
        /**
         * @brief Set the relative angle of the encoder
         *
         * This function sets the relative angle of the encoder. The relative angle is the number of rotations the
         * encoder has measured since the last reset. This function is non-blocking.
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENXIO: the port is not within the range of valid ports (1-21)
         * ENODEV: the port cannot be configured as an V5 Rotation sensor
         *
         * @param angle the relative angle to set the measured angle to
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::Rotation encoder = pros::Rotation(1);
         *     if (encoder.setAngle(0_stDeg) == 0) {
         *         std::cout << "Relative angle set!" << std::endl;
         *         std::cout < "Relative angle: " << encoder.getAngle().convert(deg) << std::endl; // outputs 0
         *     } else {
         *         std::cout << "Error setting relative angle!" << std::endl;
         *     }
         * }
         * @endcode
         */
        int setAngle(Angle angle) override;
    private:
        pros::Rotation m_encoder;
};
} // namespace lemlib