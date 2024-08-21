#pragma once

#include "hardware/Encoder/Encoder.hpp"
#include "pros/rotation.hpp"

namespace lemlib {
/**
 * @brief Encoder implementation for the V5 Rotation sensor
 *
 */
class V5RotationSensor : public Encoder {
    public:
        /**
         * @brief Construct a new V5 Rotation Sensor object
         *
         * @param encoder the pros::Rotation object to use
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::Rotation encoder = pros::Rotation(1);
         * }
         * @endcode
         */
        V5RotationSensor(pros::Rotation encoder);
        /**
         * @brief whether the V5 Rotation Sensor is connected
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
         * @brief Get the relative angle measured by the V5 Rotation Sensor
         *
         * The relative angle measured by the V5 Rotation Sensor is the angle of the encoder relative to the last time
         * the V5 Rotation Sensor was reset. As such, it is unbounded.
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
         * @brief Set the relative angle of the V5 Rotation Sensor
         *
         * This function sets the relative angle of the V5 Rotation Sensor. The relative angle is the number of
         * rotations the V5 Rotation Sensor has measured since the last reset. This function is non-blocking.
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