#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "pros/adi.hpp"

namespace lemlib {
class ADIEncoder : public Encoder {
    public:
        /**
         * @brief Construct a new ADIEncoder object
         *
         * @param topPort the top port of the ADIEncoder sensor
         * @param bottomPort the bottom port of the ADIEncoder sensor
         * @param reversed whether the encoder is reversed or not
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     // create an ADIEncoder on ports A and B, which is not reversed
         *     ADIEncoder encoder_ab = new ADIEncoder('A', 'B');
         *     // create an ADIEncoder on ports C and D, which is reversed
         *     ADIEncoder encoder_cd = new ADIEncoder('C', 'D', true);
         *     // create an ADIEncoder on ports E and F, which is not reversed
         *     ADIEncoder encoder_ef = new ADIEncoder('E', 'F', false);
         * }
         * @endcode
         */
        ADIEncoder(char topPort, char bottomPort, bool reversed = false);
        /**
         * @brief Construct a new ADIEncoder object
         *
         * @param port the ports of the ADIEncoder along with the port of the ADI expander it is connected to
         * @param reversed whether the encoder is reversed or not
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     // create an ADIEncoder on ports A and B on the expander on port 1, which is not reversed
         *     ADIEncoder encoder_ab = new ADIEncoder({1, 'A', 'B'});
         *     // create an ADIEncoder on ports C and D on the expander on port 1, which is reversed
         *     ADIEncoder encoder_cd = new ADIEncoder({1, 'C', 'D'}, true);
         *     // create an ADIEncoder on ports E and F on the expander on port 1, which is not reversed
         *     ADIEncoder encoder_ef = new ADIEncoder({1, 'E', 'F'}, false);
         * }
         * @endcode
         */
        ADIEncoder(pros::adi::ext_adi_port_tuple_t port, bool reversed = false);
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
         *     // create an ADIEncoder on ports A and B, which is not reversed
         *     pros::adi::Encoder pros_encoder_ab('A', 'B');
         *     // create an ADIEncoder on ports C and D, which is reversed
         *     pros::adi::Encoder pros_encoder_cd('C', 'D', true);
         *     // create an ADIEncoder on ports E and F, which is not reversed
         *     pros::adi::Encoder pros_encoder_ef('E', 'F', false);
         *     // create the lemlib ADIEncoder objects
         *     ADIEncoder encoder_ab(pros_encoder_ab);
         *     ADIEncoder encoder_cd(pros_encoder_cd, true);
         *     ADIEncoder encoder_ef(pros_encoder_ef, false);
         * }
         * @endcode
         */
        ADIEncoder(pros::adi::Encoder encoder, bool reversed = false);
        /**
         * @brief calibrates the encoder
         *
         * @deprecated this function is deprecated because the ADIEncoder does not support
         * calibration. If this function is called, it will act as if it was successful. However, it may still return an
         * error and set errno
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port could not be configured as an encoder
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         */
        [[deprecated("This function is not implemented as the ADIEncoder does not need to be calibrated")]]
        int calibrate() override;
        /**
         * @brief check if the encoder is calibrated
         *
         * @deprecated This function is deprecated because the ADIEncoder does not support calibration. If this function
         * is called, it will act as if it was successful. However, it may still return an error and set errno
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port could not be configured as an encoder
         *
         * @return 1 if there are no errors
         * @return INT_MAX if there is an error, setting errno
         */
        [[deprecated("This function is not implemented as the ADIEncoder does not need to be calibrated")]]
        int isCalibrated() override;
        /**
         * @brief check if the encoder is calibrating
         *
         * @deprecated This function is deprecated because the ADIEncoder does not support calibration. If this function
         * is called, it will act as if the encoder is not calibrating. However, it may still return an error and set
         * errno
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port could not be configured as an encoder
         *
         * @return 0 if there are no errors
         * @return INT_MAX if there is an error, setting errno
         */
        [[deprecated("This function is not implemented as the ADIEncoder does not need to be calibrated")]]
        int isCalibrating() override;
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
         * @brief Get the absolute angle measured by the encoder
         *
         * @deprecated This function is deprecated due to hardware limitations. Use getRelativeAngle() instead. If this
         * function is called, it will act as if getRelativeAngle() was called.
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port could not be configured as an encoder
         *
         * @return Angle the relative angle of the encoder
         * @return INFINITY if there is an error, setting errno
         */
        [[deprecated("This function is not implemented due to hardware limitations. Use getRelativeAngle() instead")]]
        Angle getAbsoluteAngle() override;
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
         *     ADIEncoder encoder('A', 'B');
         *     const Angle angle = encoder.getRelativeAngle();
         *     if (angle == INFINITY) {
         *         std::cout << "Error getting relative angle!" << std::endl;
         *     } else {
         *         std::cout << "Relative angle: " << angle.convert(deg) << std::endl;
         *     }
         * }
         * @endcode
         */
        Angle getRelativeAngle() override;
        /**
         * @brief Set the absolute zero of the encoder to the current angle
         *
         * @deprecated This function is deprecated due to hardware limitations. Use setRelativeAngle(0_stDeg) instead.
         * If this function is called, it will act like getRelativeAngle
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port could not be configured as an encoder
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     ADIEncoder encoder('A', 'B');
         *     const int result = encoder.setAbsoluteZero();
         *     if (result == 0) {
         *         std::cout << "Absolute zero set!" << std::endl;
         *         std::cout < "Absolute angle: " << encoder.getAbsoluteAngle().convert(deg) << std::endl; // outputs 0
         *     } else {
         *         std::cout << "Error setting absolute zero!" << std::endl;
         *     }
         * }
         */
        [[deprecated("This function is not implemented due to hardware limitations. Use getRelativeAngle() instead")]]
        virtual int setAbsoluteZero() override;
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
         *     ADIEncoder encoder('A', 'B');
         *     const int result = encoder.setRelativeAngle(0_stDeg);
         *     if (result == 0) {
         *         std::cout << "Relative angle set!" << std::endl;
         *         std::cout < "Relative angle: " << encoder.getRelativeAngle().convert(deg) << std::endl; // outputs 0
         *     } else {
         *         std::cout << "Error setting relative angle!" << std::endl;
         *     }
         * }
         */
        int setRelativeAngle(Angle angle) override;
        /**
         * @brief Set the encoder to be reversed
         *
         * This function sets the encoder to be reversed. This function is non-blocking.
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port could not be configured as an encoder
         *
         * @param reversed whether the encoder is reversed or not
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     ADIEncoder encoder('A', 'B', false);
         *     const int result = encoder.setReversed(true);
         *     if (result == 0) {
         *         std::cout << "Encoder reversed!" << std::endl;
         *         encoder.isReversed(); // outputs 1
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
         * ENODEV: the port could not be configured as an encoder
         *
         * @return 0 the encoder is not reversed
         * @return 1 the encoder is reversed
         * @return INT_MAX if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     ADIEncoder encoder('A', 'B', true);
         *     const int result = encoder.isReversed();
         *     if (result == 1) {
         *         // this is the expected output
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
         * @return pros::adi::ext_adi_port_tuple_t
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     // example 1: encoder on ports A and B
         *     ADIEncoder encoder_ab('A', 'B');
         *     const pros::adi::ext_adi_port_tuple_t port = encoder.getPort();
         *     std::cout << "Top port: " << std::get<1>(port) << std::endl; // outputs 'A'
         *     std::cout << "Bottom port: " << std::get<2>(port) << std::endl; // outputs 'B'
         *     // example 2: encoder on ports C and D on the expander on port 1
         *     ADIEncoder encoder_cd({1, 'C', 'D'});
         *     const pros::adi::ext_adi_port_tuple_t port = encoder.getPort();
         *     std::cout << "Expander port: " << std::get<0>(port) << std::endl; // outputs 1
         *     std::cout << "Top port: " << std::get<1>(port) << std::endl; // outputs 'C'
         *     std::cout << "Bottom port: " << std::get<2>(port) << std::endl; // outputs 'D'
         * }
         * @endcode
         */
        pros::adi::ext_adi_port_tuple_t getPort() const;
    protected:
        pros::adi::Encoder m_encoder;
        Angle m_offset = 0_stDeg;
    private:
        bool m_reversed;
};
} // namespace lemlib