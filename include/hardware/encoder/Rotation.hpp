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
         * @param port the signed port of the encoder. If the port is negative, the encoder will be reversed
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     // create a new Rotation encoder on port 1 that is not reversed
         *     Rotation encoder_1(1);
         *     // create a new Rotation encoder on port 2 that is reversed
         *     Rotation encoder_2(-2);
         * }
         * @endcode
         */
        Rotation(int port);
        /**
         * @brief Construct a new Rotation encoder object
         *
         * @param port the port of the encoder
         * @param reversed whether the encoder should be reversed or not
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     // create a new Rotation encoder on port 1 that is not reversed
         *     Rotation encoder_1(1, false);
         *     // create a new Rotation encoder on port 2 that is reversed
         *     Rotation encoder_2(2, true);
         *     // create a new Rotation encoder on port 3 that is not reversed
         *     Rotation encoder_3(3);
         * }
         * @endcode
         */
        Rotation(std::uint8_t port, bool reversed = false);
        /**
         * @brief Construct a new Rotation encoder object
         *
         * @param encoder the pros::Rotation object to use
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Rotation encoder = pros::Rotation(1);
         *     Rotation encoder_1(encoder);
         * }
         * @endcode
         */
        Rotation(pros::Rotation encoder);
        /**
         * @brief whether the encoder is connected
         *
         *
         * @return 0 if its not connected
         * @return 1 if it is connected
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Rotation encoder(1);
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
         *     Rotation encoder(1);
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
         *     Rotation encoder(1);
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
        /**
         * @brief Set the encoder to be reversed
         *
         * This function sets the encoder to be reversed. This function is non-blocking.
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENXIO: the port is not within the range of valid ports (1-21)
         * ENODEV: the port cannot be configured as an V5 Rotation sensor
         *
         * @param reversed whether the encoder should be reversed or not
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Rotation encoder(1);
         *     if (encoder.setReversed(true) == 0) {
         *         std::cout << "Encoder reversed!" << std::endl;
         *     } else {
         *         std::cout << "Error reversing encoder!" << std::endl;
         *     }
         * }
         * @endcode
         */
        int setReversed(bool reversed) override;
        /**
         * @brief Check if the encoder is reversed
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENXIO: the port is not within the range of valid ports (1-21)
         * ENODEV: the port cannot be configured as an V5 Rotation sensor
         *
         * @return 0 if its not reversed
         * @return 1 if it is reversed
         * @return INT_MAX if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Rotation encoder(1);
         *     const int result = encoder.isReversed();
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
        int isReversed() override;
        /**
         * @brief Get the port of the encoder
         *
         * @return int
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Rotation encoder(1);
         *     std::cout << "Encoder port: " << encoder.getPort() << std::endl;
         * }
         * @endcode
         */
        int getPort() const;
    protected:
        pros::Rotation m_encoder;
};
} // namespace lemlib