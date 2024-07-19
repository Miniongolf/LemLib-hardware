#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "units/Angle.hpp"

namespace lemlib {

enum BrakeMode { COAST = 0, BRAKE = 1, HOLD = 2 };

class AbstractMotor : public Encoder {
    public:
        /**
         * @brief move the motor at a percent power from -1.0 to +1.0
         *
         * @param percent the power to move the motor at from -1.0 to +1.0
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Motor* motor = new Motor(1);
         *     // move the motor forward at 50% power
         *     motor->move(0.5);
         *     // move the motor backward at 50% power
         *     motor->move(-0.5);
         *     // stop the motor
         *     motor->move(0);
         * }
         * @endcode
         */
        virtual int move(double percent) = 0;
        /**
         * @brief move the motor at a given angular velocity
         *
         * @param velocity the target angular velocity to move the motor at
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Motor* motor = new Motor(1);
         *     // move the motor forward at 50 degrees per second
         *     motor->moveVelocity(50_degps);
         *     // move the motor backward at 50 degrees per second
         *     motor->moveVelocity(-50_degps);
         *     // stop the motor
         *     motor->moveVelocity(0_degps);
         * }
         * @endcode
         */
        virtual int moveVelocity(AngularVelocity velocity) = 0;
        /**
         * @brief brake the motor
         *
         * This function will stop the motor using the set brake mode
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Motor* motor = new Motor(1);
         *     // move the motor forward at 50% power
         *     motor->move(0.5);
         *     // brake the motor
         *     motor->brake();
         * }
         * @endcode
         */
        virtual int brake() = 0;
        /**
         * @brief set the brake mode of the motor
         *
         * @param mode the brake mode to set the motor to
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Motor* motor = new Motor(1);
         *     // set the motor to brake when stopped
         *     motor->setBrakeMode(BrakeMode::BRAKE);
         *     // set the motor to coast when stopped
         *     motor->setBrakeMode(BrakeMode::COAST);
         *     // set the motor to hold when stopped
         *     motor->setBrakeMode(BrakeMode::HOLD);
         * }
         * @endcode
         */
        virtual int setBrakeMode(BrakeMode mode) = 0;
        /**
         * @brief get the brake mode of the motor
         *
         * @return BrakeMode enum value of the brake mode
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     Motor* motor = new Motor(1);
         *     const BrakeMode mode = motor->getBrakeMode();
         *     if (mode == BrakeMode::BRAKE) {
         *         std::cout << "Brake mode is set to BRAKE!" << std::endl;
         *     } else if (mode == BrakeMode::COAST) {
         *         std::cout << "Brake mode is set to COAST!" << std::endl;
         *     } else if (mode == BrakeMode::HOLD) {
         *         std::cout << "Brake mode is set to HOLD!" << std::endl;
         *     } else {
         *         std::cout << "Error getting brake mode!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual BrakeMode getBrakeMode() = 0;
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
         *     Encoder* encoder = new Motor(1);
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
         *     Encoder* encoder = new Motor(1);
         *     const Angle angle = encoder->getAngle();
         *     if (angle == INFINITY) {
         *         std::cout << "Error getting relative angle!" << std::endl;
         *     } else {
         *         std::cout << "Relative angle: " << angle.convert(deg) << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual Angle getAngle() = 0;
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
         *     Encoder* encoder = new Motor(1);
         *     if (encoder->setAngle(0_stDeg) == 0) {
         *         std::cout << "Relative angle set!" << std::endl;
         *         std::cout < "Relative angle: " << encoder->getAngle().convert(deg) << std::endl; // outputs 0
         *     } else {
         *         std::cout << "Error setting relative angle!" << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual int setAngle(Angle angle) = 0;
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
         *     Encoder* encoder = new Motor(1);
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
         *     Encoder* encoder = new Motor(1);
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
        virtual ~AbstractMotor() = default;
};
} // namespace lemlib