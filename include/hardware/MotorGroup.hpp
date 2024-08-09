#include "hardware/Motor.hpp"
#include "pros/motor_group.hpp"
#include <vector>
#include <initializer_list>

namespace lemlib {
/**
 * @brief MotorGroup class
 *
 * This class is a handler for a group of lemlib::Motor objects, which themselves are wrappers for ther pros::Motor
 * objects. This class allows for easy control of collection of telemetry, as inputs and outputs are unitized. This
 * class also enables users to add and remove motors from the group, which is useful when a motor can be moved between
 * subsystems using a Power Take Off (PTO) or similar mechanism.
 *
 * Error handling for the MotorGroup class is a bit different from other hardware classes. This is because
 * the MotorGroup class represents a group of motors, any of which could fail. However, as long as one
 * motor in the group is functioning properly, the MotorGroup will not throw any errors. Since motors could fail
 * for different reasons, the behaviour of errno is to be considered undefined after a failure.
 */
class MotorGroup : Encoder {
    public:
        /**
         * @brief Construct a new Motor Group
         *
         * @param ports list of ports of the motors in the group
         * @param outputVelocity the theoretical maximum output velocity of the motor group, after gearing
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         * }
         * @endcode
         */
        MotorGroup(std::initializer_list<pros::Motor> motors, AngularVelocity outputVelocity);
        /**
         * @brief Construct a new Motor Group
         *
         * @param motors PROS motor group object
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *     lemlib::MotorGroup motorGroup(motorGroup);
         * }
         * @endcode
         */
        MotorGroup(pros::v5::MotorGroup motors, AngularVelocity outputVelocity);
        /**
         * @brief move the motors at a percent power from -1.0 to +1.0
         *
         * @param percent the power to move the motors at from -1.0 to +1.0
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *     // move the motors forward at 50% power
         *     motorGroup.move(0.5);
         *     // move the motors backward at 50% power
         *     motorGroup.move(-0.5);
         *     // stop the motors
         *     motorGroup.move(0);
         * }
         * @endcode
         */
        int move(double percent);
        /**
         * @brief move the motors at a given angular velocity
         *
         * @param velocity the target angular velocity to move the motors at
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *     // move the motors forward at 50 degrees per second
         *     motorGroup.moveVelocity(50_degps);
         *     // move the motors backward at 50 degrees per second
         *     motorGroup.moveVelocity(-50_degps);
         *     // stop the motors
         *     motorGroup.moveVelocity(0_degps);
         * }
         * @endcode
         */
        int moveVelocity(AngularVelocity velocity);
        /**
         * @brief brake the motors
         *
         * This function will stop the motors using the set brake mode
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *     // move the motors forward at 50% power
         *     motorGroup.move(0.5);
         *     // brake the motors
         *     motorGroup.brake();
         * }
         * @endcode
         */
        int brake();
        /**
         * @brief set the brake mode of the motors
         *
         * @param mode the brake mode to set the motors to
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *     // set the motors to brake when stopped
         *     motorGroup.setBrakeMode(lemlib::BrakeMode::BRAKE);
         *     // set the motors to coast when stopped
         *     motorGroup.setBrakeMode(lemlib::BrakeMode::COAST);
         *     // set the motors to hold when stopped
         *     motorGroup.setBrakeMode(lemlib::BrakeMode::HOLD);
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
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *     const lemlib::BrakeMode mode = motorGroup.getBrakeMode();
         *     if (mode == lemlib::BrakeMode::BRAKE) {
         *         std::cout << "Brake mode is set to BRAKE!" << std::endl;
         *     } else if (mode == lemlib::BrakeMode::COAST) {
         *         std::cout << "Brake mode is set to COAST!" << std::endl;
         *     } else if (mode == lemlib::BrakeMode::HOLD) {
         *         std::cout << "Brake mode is set to HOLD!" << std::endl;
         *     } else {
         *         std::cout << "Error getting brake mode!" << std::endl;
         *     }
         * }
         * @endcode
         */
        std::vector<BrakeMode> getBrakeMode();
        /**
         * @brief whether any of the motors in the motor group are connected
         *
         * @return 0 no motors are connected
         * @return 1 if at least one motor is connected
         * @return INT_MAX if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *     const int result = motorGroup.isConnected();
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
         * @brief Get the average relative angle measured by the motors
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
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *     const Angle angle = motorGroup.getAngle();
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
         * @brief Set the relative angle of all the motors
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
         *     pros::Motor motor1(1);
         *     pros::Motor motor2(2);
         *     pros::Motor motor3(3);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *     if (motorGroup.setAngle(0_stDeg) == 0) {
         *         std::cout << "Relative angle set!" << std::endl;
         *         std::cout < "Relative angle: " << motorGroup.getAngle().convert(deg) << std::endl; // outputs 0
         *     } else {
         *         std::cout << "Error setting relative angle!" << std::endl;
         *     }
         * }
         * @endcode
         */
        int setAngle(Angle angle) override;
    private:
        const AngularVelocity m_outputVelocity;
        std::vector<Motor> m_motors;
};
}; // namespace lemlib