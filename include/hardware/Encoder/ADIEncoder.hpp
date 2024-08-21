#pragma once

#include "hardware/Encoder/Encoder.hpp"
#include "pros/adi.hpp"

namespace lemlib {
/**
 * @brief Encoder implementation for the Optical Shaft Encoder
 *
 */
class ADIEncoder : public Encoder {
    public:
        /**
         * @brief Construct a new ADIEncoder object
         *
         * Even though VEXOS knows whether the encoder is reversed, it has no API to get this information. As such, its
         * necessary to pass this info to the constructor.
         *
         * @param encoder the pros::ADIEncoder object to use
         * @param reversed whether the encoder is reversed or not
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::ADIEncoder encoder = pros::adi::Encoder('A', 'B');
         * }
         */
        ADIEncoder(pros::adi::Encoder encoder);
        /**
         * @brief whether the encoder is connected
         *
         * @deprecated This function is deprecated because there is no way to check if the ADIEncoder is connected due
         * to the nature of the ADI ports. If this function is called, it will act as if the encoder is connected.
         * However, it may still return an error and set errno
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port could not be configured as an encoder
         *
         * @return 1 if there are no errors
         * @return INT_MAX if there is an error, setting errno
         */
        [[deprecated("This function is not implemented due to hardware limitations")]]
        int isConnected() override;
        /**
         * @brief Get the relative angle measured by the encoder
         *
         * The relative angle measured by the encoder is the angle of the encoder relative to the last time the encoder
         * was reset. As such, it is unbounded.
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port could not be configured as an encoder
         *
         * @return Angle the relative angle of the encoder
         * @return INFINITY if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::ADIEncoder encoder = pros::adi::Encoder('A', 'B');
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
         * ENODEV: the port could not be configured as an encoder
         *
         * @param angle the angle to set the measured angle to
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::ADIEncoder encoder = pros::adi::Encoder('A', 'B');
         *     const int result = encoder.setAngle(0_stDeg);
         *     if (result == 0) {
         *         std::cout << "Relative angle set!" << std::endl;
         *         std::cout < "Relative angle: " << encoder.getAngle().convert(deg) << std::endl; // outputs 0
         *     } else {
         *         std::cout << "Error setting relative angle!" << std::endl;
         *     }
         * }
         */
        int setAngle(Angle angle) override;
    private:
        pros::adi::Encoder m_encoder;
        Angle m_offset = 0_stDeg;
};
} // namespace lemlib