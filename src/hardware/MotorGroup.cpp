#include "hardware/MotorGroup.hpp"

namespace lemlib {
MotorGroup::MotorGroup(std::initializer_list<pros::Motor> motors) {
    for (const pros::Motor motor : motors) m_motors.push_back(motor);
}

MotorGroup::MotorGroup(std::initializer_list<lemlib::Motor> motors) {
    for (const lemlib::Motor motor : motors) m_motors.push_back(motor);
}

MotorGroup::MotorGroup(pros::v5::MotorGroup motors) {
    const int size = motors.size();
    for (int i = 0; i < size; i++) m_motors.push_back(pros::Motor(motors.get_port(i)));
}

MotorGroup::MotorGroup(std::initializer_list<int> ports, pros::MotorGears gear) {
    for (const int port : ports) m_motors.push_back(pros::Motor(port, gear));
}
}; // namespace lemlib