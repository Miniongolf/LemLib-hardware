#include "hardware/MotorGroup.hpp"
#include "units/Angle.hpp"
#include <climits>

namespace lemlib {
MotorGroup::MotorGroup(std::initializer_list<pros::Motor> motors, AngularVelocity outputVelocity)
    : m_outputVelocity(outputVelocity) {
    for (const pros::Motor motor : motors) m_motors.push_back(motor);
}

MotorGroup::MotorGroup(pros::v5::MotorGroup motors, AngularVelocity outputVelocity)
    : m_outputVelocity(outputVelocity) {
    const int size = motors.size();
    for (int i = 0; i < size; i++) m_motors.push_back(pros::Motor(motors.get_port(i)));
}

int MotorGroup::move(double percent) {
    bool success = false;
    for (Motor motor : m_motors) {
        const int result = motor.move(percent);
        if (result == 0) success = true;
    }
    // as long as one motor moves successfully, return 0 (success)
    return success ? 0 : INT_MAX;
}

int MotorGroup::moveVelocity(AngularVelocity velocity) {
    bool success = false;
    for (Motor motor : m_motors) {
        // since the motors in the group are geared together, we need to account for different gearings
        // of different motors in the group
        const int result = motor.moveVelocity(velocity);
        if (result == 0) success = true;
    }
    // as long as one motor moves successfully, return 0 (success)
    return success ? 0 : INT_MAX;
}
}; // namespace lemlib