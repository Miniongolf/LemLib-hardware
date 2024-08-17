#include "hardware/Motors/Motor.hpp"
#include "pros/motor_group.hpp"

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
         *     // in order for automatic gear ration calculations to work properly, the motors must
         *     // have a cartridge passed to their constructor
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     // create a new motor group with motors 1, 2, and 3
         *     // with a theoretical maximum output velocity of 200 rpm
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
         *     // in order for automatic gear ration calculations to work properly, the motors must
         *     // have a cartridge passed to their constructor
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     pros::MotorGroup motorGroup({motor1, motor2, motor3});
         *     // create a new motor group with motors 1, 2, and 3
         *     // with a theoretical maximum output velocity of 200 rpm
         *     lemlib::MotorGroup motorGroup(motorGroup, 200_rpm);
         * }
         * @endcode
         */
        MotorGroup(pros::v5::MotorGroup motors, AngularVelocity outputVelocity);
        /**
         * @brief move the motors at a percent power from -1.0 to +1.0
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motor
         *
         * @param percent the power to move the motors at from -1.0 to +1.0
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
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
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motor
         *
         * @param velocity the target angular velocity to move the motors at
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
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
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motor
         *
         * This function will stop the motors using the set brake mode
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
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
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motor
         *
         * @param mode the brake mode to set the motors to
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
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
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motor
         *
         * @return BrakeMode enum value of the brake mode
         * @return BrakeMode::INVALID on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
         *     const lemlib::BrakeMode mode = motorGroup.getBrakeModes();
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
        std::vector<BrakeMode> getBrakeModes();
        /**
         * @brief whether any of the motors in the motor group are connected
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motor
         *
         * @return 0 no motors are connected
         * @return 1 if at least one motor is connected
         * @return INT_MAX if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
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
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motor
         *
         * @return Angle the relative angle measured by the encoder
         * @return INFINITY if there is an error, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
         *     const Angle angle = motorGroup.getAngle();
         *     if (angle == INFINITY) {
         *         std::cout << "Error getting relative angle!" << std::endl;
         *     } else {
         *         std::cout << "Relative angle: " << to_sDeg(angle) << std::endl;
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
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motor
         *
         * @param angle the relative angle to set the measured angle to
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
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
        /**
         * @brief Get the number of connected motors in the group
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motors
         *
         * @return int the number of connected motors in the group
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
         *     std::cout << "Number of connected motors: " << motorGroup.getSize() << std::endl;
         * }
         * @endcode
         */
        int getSize() const;
        /**
         * @brief Add a motor to the motor group
         *
         * This function adds a motor to the motor group. If successful, it will set the angle measured by the motor to
         * the average angle measured by the motor group. It will also set the brake mode of the motor to that of the
         * first working motor in the group. If there are any errors, the motor will not be added to the group.
         *
         * This function uses the following values of errno when an error state is reached:
         *
         * ENODEV: the port cannot be configured as a motor
         *
         * @param port the signed port of the motor to be added to the group. Negative ports indicate the motor should
         * be reversed
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
         *     // add a motor to the group
         *     motorGroup.addMotor(4);
         * }
         * @endcode
         */
        int addMotor(int port);
        /**
         * @brief Add a motor to the motor group
         *
         * This function adds a motor to the motor group. If successful, it will set the angle measured by the motor to
         * the average angle measured by the motor group. It will also set the brake mode of the motor to that of the
         * first working motor in the group. If there are any errors, the motor will not be added to the group.
         *
         * @param motor the motor to be added to the group
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
         *    // add a motor to the group
         *     pros::Motor motor4(4, pros::v5::MotorGears::green);
         *     motorGroup.addMotor(motor4);
         * }
         * @endcode
         */
        int addMotor(Motor motor);
        /**
         * @brief Add a motor to the motor group
         *
         * This function adds a motor to the motor group. If successful, it will set the angle measured by the motor to
         * the average angle measured by the motor group. It will also set the brake mode of the motor to that of the
         * first working motor in the group. If there are any errors, the motor will not be added to the group.
         *
         * @param motor the motor to be added to the group
         * @param reversed whether the motor should be reversed
         *
         * @return 0 on success
         * @return INT_MAX on failure, setting errno
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
         *    // add a motor to the group, which should be reversed
         *     pros::Motor motor4(4, pros::v5::MotorGears::green);
         *     motorGroup.addMotor(motor4, true);
         * }
         * @endcode
         */
        int addMotor(Motor motor, bool reversed);
        /**
         * @brief Remove a motor from the motor group
         *
         * This function does not return any errors
         *
         * @param port the port of the motor to be removed from the group
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
         *     // remove a motor from the group
         *     motorGroup.removeMotor(4);
         * }
         * @endcode
         */
        void removeMotor(int port);
        /**
         * @brief Remove a motor from the motor group
         *
         * This function does not return any errors
         *
         * @param motor the motor to be removed from the group
         *
         * @b Example:
         * @code {.cpp}
         * void initialize() {
         *     pros::Motor motor1(1, pros::v5::MotorGears::green);
         *     pros::Motor motor2(2, pros::v5::MotorGears::green);
         *     pros::Motor motor3(3, pros::v5::MotorGears::green);
         *     lemlib::MotorGroup motorGroup({motor1, motor2, motor3}, 200_rpm);
         *
         *     // remove a motor from the group
         *     pros::Motor motor4(4, pros::v5::MotorGears::green);
         *     motorGroup.removeMotor(motor4);
         * }
         * @endcode
         */
        void removeMotor(Motor motor);
    private:
        /**
         * @brief Get motors in the motor group as a vector of lemlib::Motor objects
         *
         * This function exists to simplify logic in the MotorGroup source code.
         *
         * @return const std::vector<Motor> vector of lemlib::Motor objects
         */
        const std::vector<Motor> getMotors() const;
        const AngularVelocity m_outputVelocity;
        /**
         * This member variable is a vector of motor ports
         *
         * Ideally, we'd use a vector of lemlib::Motor objects, but this does not work if you want to remove an element
         * from the vector as the copy constructor is implicitly deleted.
         *
         * The ports are signed to indicate whether a motor should be reversed or not.
         */
        std::vector<int8_t> m_ports;
};
}; // namespace lemlib