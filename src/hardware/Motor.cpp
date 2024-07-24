#include "hardware/Motor.hpp"
#include "pros/abstract_motor.hpp"

namespace lemlib {
Motor::Motor(pros::Motor motor)
    : m_motor(motor) {}

int Motor::move(double percent) {
    // the V5 and EXP motors have different voltage caps, so we need to scale based on the motor type
    // V5 motors have their voltage capped at 12v, while EXP motors have their voltage capped at 7.2v
    // but they have the same max velocity, so we can scale the percent power based on the motor type
    if (getType() == MotorType::V5) {
        m_motor.move_voltage(percent * 12000);
    } else {
        m_motor.move_voltage(percent * 7200);
    }
}

MotorType Motor::getType() {
    // there is no exposed api to get the motor type
    // while the memory address of the private member variable has been found through reverse engineering,
    // it may break between VEXos updates. Instead, we see if we can change the cartridge to something other
    // than the green cartridge, which is only possible on the V5 motor
    const auto oldCart = m_motor.get_gearing();
    m_motor.set_gearing(pros::v5::MotorGears::red);
    if (m_motor.get_gearing() == pros::v5::MotorGears::red) {
        m_motor.set_gearing(oldCart);
        return MotorType::V5;
    } else return MotorType::EXP;
}
} // namespace lemlib