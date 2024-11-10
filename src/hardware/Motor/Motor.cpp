#include "hardware/Motor/Motor.hpp"
#include "hardware/util.hpp"
#include "units/Angle.hpp"
#include "pros/device.h"
#include "pros/motors.h"

using namespace pros;
using namespace pros::c;

namespace lemlib {
Motor::Motor(int port) {
    m_port = port;
}

Motor::Motor(uint8_t port, bool reversed) {
    if (reversed) m_port = -port;
    else m_port = port;
}

motor_brake_mode_e_t brakeModeToMotorBrake(BrakeMode mode) {
    // MotorBrake is identical to lemlib::BrakeMode, except for its name and lemlib uses an enum class for type
    // safety
    switch (mode) {
        case BrakeMode::COAST: return motor_brake_mode_e_t::E_MOTOR_BRAKE_COAST;
        case BrakeMode::BRAKE: return motor_brake_mode_e_t::E_MOTOR_BRAKE_BRAKE;
        case BrakeMode::HOLD: return motor_brake_mode_e_t::E_MOTOR_BRAKE_HOLD;
        default: return motor_brake_mode_e_t::E_MOTOR_BRAKE_INVALID;
    }
}

BrakeMode motorBrakeToBrakeMode(motor_brake_mode_e_t mode) {
    // MotorBrake is identical to lemlib::BrakeMode, except for its name and lemlib uses an enum class for type
    // safety
    switch (mode) {
        case motor_brake_mode_e_t::E_MOTOR_BRAKE_COAST: return BrakeMode::COAST;
        case motor_brake_mode_e_t::E_MOTOR_BRAKE_BRAKE: return BrakeMode::BRAKE;
        case motor_brake_mode_e_t::E_MOTOR_BRAKE_HOLD: return BrakeMode::HOLD;
        default: return BrakeMode::INVALID;
    }
}

int Motor::move(double percent) {
    // the V5 and EXP motors have different voltage caps, so we need to scale based on the motor type
    // V5 motors have their voltage capped at 12v, while EXP motors have their voltage capped at 7.2v
    // but they have the same max velocity, so we can scale the percent power based on the motor type
    switch (getType()) {
        case (MotorType::V5): return convertStatus(motor_move_voltage(m_port, percent * 12000));
        case (MotorType::EXP): return convertStatus(motor_move_voltage(m_port, percent * 7200));
        default: return INT_MAX;
    }
}

int Motor::moveVelocity(AngularVelocity velocity) {
    // pros uses an integer value to represent the rpm of the motor
    return convertStatus(motor_move_velocity(m_port, to_rpm(units::round(velocity, rpm))));
}

int Motor::brake() { return convertStatus(motor_brake(m_port)); }

int Motor::setBrakeMode(BrakeMode mode) { return convertStatus(motor_set_brake_mode(m_port, brakeModeToMotorBrake(mode))); }

BrakeMode Motor::getBrakeMode() const { return motorBrakeToBrakeMode(motor_get_brake_mode(m_port)); }

int Motor::isConnected() { return get_plugged_type(m_port) == v5_device_e_t::E_DEVICE_MOTOR; }

Angle Motor::getAngle() {
    // get the raw position
    const int ticks = motor_get_raw_position(m_port, NULL);
    if (ticks == INT_MAX) return from_stRot(INFINITY);
    // get the cartridge
    const Cartridge cartridge = getCartridge();
    if (cartridge == Cartridge::INVALID) return from_stRot(INFINITY);
    // convert the raw position into an angle
    const int tpr = 50 * 3600 / static_cast<int>(cartridge);
    const Angle position = from_stRot(double(ticks) / double(tpr));
    // return position + offset
    return position + m_offset;
}

int Motor::setAngle(Angle angle) {
    // get the raw position
    const int ticks = motor_get_raw_position(m_port, NULL);
    if (ticks == INT_MAX) return INT_MAX;
    // get the cartridge
    const Cartridge cartridge = getCartridge();
    if (cartridge == Cartridge::INVALID) return INT_MAX;
    // convert the raw position into an angle
    const int tpr = 50 * 3600 / static_cast<int>(cartridge);
    const Angle position = from_stRot(double(ticks) / double(tpr));
    // calculate offset
    m_offset = angle - position;
    return 0;
}

Angle Motor::getOffset() const { return m_offset; }

int Motor::setOffset(Angle offset) {
    m_offset = offset;
    return 0;
}

MotorType Motor::getType() {
    // there is no exposed api to get the motor type
    // while the memory address of the function has been found through reverse engineering,
    // it may break between VEXos updates. Instead, we see if we can change the cartridge to something other
    // than the green cartridge, which is only possible on the V5 motor
    const motor_gearset_e_t oldCart = motor_get_gearing(m_port);
    const int result = motor_set_gearing(m_port, motor_gearset_e_t::E_MOTOR_GEAR_RED);
    // check for errors
    if (oldCart == motor_gearset_e_t::E_MOTOR_GEARSET_INVALID) return MotorType::INVALID;
    if (result == INT_MAX) return MotorType::INVALID;
    // check if the gearing changed or not
    const motor_gearset_e_t newCart = motor_get_gearing(m_port);
    if (newCart == motor_gearset_e_t::E_MOTOR_GEARSET_INVALID) return MotorType::INVALID;
    if (newCart != motor_gearset_e_t::E_MOTOR_GEAR_GREEN) {
        // set the cartridge back to its original value
        if (motor_set_gearing(m_port, oldCart) == INT_MAX) return MotorType::INVALID;
        else return MotorType::V5;
    } else return MotorType::EXP;
}

Cartridge Motor::getCartridge() const {
    const motor_gearset_e_t cartridge = motor_get_gearing(m_port);
    // convert the cartridge to our enum
    switch (cartridge) {
        case E_MOTOR_GEAR_BLUE: return Cartridge::BLUE;
        case E_MOTOR_GEAR_GREEN: return Cartridge::GREEN;
        case E_MOTOR_GEAR_RED: return Cartridge::RED;
        default: return Cartridge::INVALID;
    }
}

int Motor::isReversed() const {
    // technically this returns an int, but as long as you only pass 0 to the index its impossible for it to return an
    // error. This is because we keep track of whether the motor is reversed or not through the sign of its port
    return m_port < 0;
}

int Motor::setReversed(bool reversed) {
    // technically this returns an int, but as long as you only pass 0 to the index its impossible for it to return an
    // error. This is because we keep track of whether the motor is reversed or not through the sign of its port
    if (reversed) {
        m_port = -abs(m_port);
    } else {
        m_port = abs(m_port);
    }
    return 0;
}

int Motor::getPort() const { return m_port; }

Temperature Motor::getTemperature() const { return units::from_celsius(motor_get_temperature(m_port)); }
} // namespace lemlib