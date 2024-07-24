#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "pros/motors.hpp"

namespace lemlib {

enum class BrakeMode { COAST, BRAKE, HOLD, INVALID };

enum class MotorType { V5, EXP, INVALID };

class Motor : public Encoder {
    public:
        /**
         * @brief Construct a new Motor object
         *
         * @param port the port of the motor
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     // construct a new Motor object with port 1
         *     lemlib::Motor motor = pros::Motor(1);
         * }
         * @endcode
         */
        Motor(pros::Motor motor);
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
         *     lemlib::Motor motor = pros::Motor(1);
         *     // move the motor forward at 50% power
         *     motor.move(0.5);
         *     // move the motor backward at 50% power
         *     motor.move(-0.5);
         *     // stop the motor
         *     motor.move(0);
         * }
         * @endcode
         */
        int move(double percent);
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
         *     lemlib::Motor motor = pros::Motor(1);
         *     // move the motor forward at 50 degrees per second
         *     motor.moveVelocity(50_degps);
         *     // move the motor backward at 50 degrees per second
         *     motor.moveVelocity(-50_degps);
         *     // stop the motor
         *     motor.moveVelocity(0_degps);
         * }
         * @endcode
         */
        int moveVelocity(AngularVelocity velocity);
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
         *     lemlib::Motor motor = pros::Motor(1);
         *     // move the motor forward at 50% power
         *     motor.move(0.5);
         *     // brake the motor
         *     motor.brake();
         * }
         * @endcode
         */
        int brake();
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
         *     lemlib::Motor motor = pros::Motor(1);
         *     // set the motor to brake when stopped
         *     motor.setBrakeMode(BrakeMode::BRAKE);
         *     // set the motor to coast when stopped
         *     motor.setBrakeMode(BrakeMode::COAST);
         *     // set the motor to hold when stopped
         *     motor.setBrakeMode(BrakeMode::HOLD);
         * }
         * @endcode
         */
        int setBrakeMode(BrakeMode mode);
        /**
         * @brief get the brake mode of the motor
         *
         * @return BrakeMode enum value of the brake mode
         * @return BrakeMode::INVALID on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::Motor motor = pros::Motor(1);
         *     const BrakeMode mode = motor.getBrakeMode();
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
        BrakeMode getBrakeMode();
        /**
         * @brief whether the motor is connected
         *
         * @return 0 if its not connected
         * @return 1 if it is connected
         * @return INT_MAX if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::Motor motor = pros::Motor(1);
         *     const int result = motor.isConnected();
         *     if (result == 1) {
         *         std::cout << "motor is connected!" << std::endl;
         *     } else if (result == 0) {
         *         std::cout << "motor is not connected!" << std::endl;
         *     } else {
         *         std::cout << "Error checking if motor is connected!" << std::endl;
         *     }
         * }
         * @endcode
         */
        int isConnected() override;
        /**
         * @brief Get the relative angle measured by the motor
         *
         * The relative angle measured by the motor is the angle of the motor relative to the last time the motor
         * was reset. As such, it is unbounded.
         *
         * @return Angle the relative angle measured by the motor
         * @return INFINITY if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::Motor motor = pros::Motor(1);
         *     const Angle angle = motor.getAngle();
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
         * @brief Set the relative angle of the motor
         *
         * This function sets the relative angle of the motor. The relative angle is the number of rotations the
         * motor has measured since the last reset. This function is non-blocking.
         *
         * @param angle the relative angle to set the measured angle to
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     lemlib::Motor motor = pros::Motor(1);
         *     if (motor.setAngle(0_stDeg) == 0) {
         *         std::cout << "Relative angle set!" << std::endl;
         *         std::cout < "Relative angle: " << motor.getAngle().convert(deg) << std::endl; // outputs 0
         *     } else {
         *         std::cout << "Error setting relative angle!" << std::endl;
         *     }
         * }
         * @endcode
         */
        int setAngle(Angle angle) override;
        /**
         * @brief Get the type of the motor
         *
         * There are 2 motors legal for use: The 11W V5 motor and the 5.5W EXP motor
         *
         * @return MotorType the type of the motor
         * @return MotorType::Invalid on failure, setting errno
         */
        MotorType getType();
    private:
        pros::Motor m_motor;
};
} // namespace lemlib