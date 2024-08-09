#include "hardware/MotorGroup.hpp"
#include "units/Angle.hpp"

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
}; // namespace lemlib