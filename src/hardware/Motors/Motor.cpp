#include "hardware/Motors/Motor.hpp"
#include "hardware/util.hpp"
#include "pros/abstract_motor.hpp"
#include "units/Angle.hpp"

namespace lemlib {
pros::MotorBrake brakeModeToMotorBrake(BrakeMode mode) {
    // pros::MotorBrake is identical to lemlib::BrakeMode, except for its name and lemlib uses an enum class for type
    // safety
    switch (mode) {
        case BrakeMode::COAST: return pros::MotorBrake::coast;
        case BrakeMode::BRAKE: return pros::MotorBrake::brake;
        case BrakeMode::HOLD: return pros::MotorBrake::hold;
        default: return pros::MotorBrake::invalid;
    }
}

BrakeMode motorBrakeToBrakeMode(pros::MotorBrake mode) {
    // pros::MotorBrake is identical to lemlib::BrakeMode, except for its name and lemlib uses an enum class for type
    // safety
    switch (mode) {
        case pros::MotorBrake::coast: return BrakeMode::COAST;
        case pros::MotorBrake::brake: return BrakeMode::BRAKE;
        case pros::MotorBrake::hold: return BrakeMode::HOLD;
        default: return BrakeMode::INVALID;
    }
}

Motor::Motor(pros::Motor motor)
    : m_motor(motor) {}

int Motor::move(double percent) {
    // the V5 and EXP motors have different voltage caps, so we need to scale based on the motor type
    // V5 motors have their voltage capped at 12v, while EXP motors have their voltage capped at 7.2v
    // but they have the same max velocity, so we can scale the percent power based on the motor type
    switch (getType()) {
        case (MotorType::V5): return convertStatus(m_motor.move_voltage(percent * 12000));
        case (MotorType::EXP): return convertStatus(m_motor.move_voltage(percent * 7200));
        default: return INT_MAX;
    }
}

int Motor::moveVelocity(AngularVelocity velocity) {
    // pros uses an integer value to represent the rpm of the motor
    return convertStatus(m_motor.move_velocity(to_rpm(units::round(velocity, rpm))));
}

int Motor::brake() { return convertStatus(m_motor.brake()); }

int Motor::setBrakeMode(BrakeMode mode) { return convertStatus(m_motor.set_brake_mode(brakeModeToMotorBrake(mode))); }

BrakeMode Motor::getBrakeMode() const { return motorBrakeToBrakeMode(m_motor.get_brake_mode()); }

int Motor::isConnected() { return m_motor.is_installed(); }

Angle Motor::getAngle() {
    // to allow the user to use whatever encoder units they want, we only deal with raw encoder counts
    // and adjust for different cartridges ourselves
    const pros::MotorGears cartridge = m_motor.get_gearing();
    const int counts = getAbsoluteCounts();
    if (counts == INT_MAX) return from_stDeg(INFINITY);
    switch (cartridge) {
        // without a cartridge, the encoder measures 50 counts per revolution
        // so we multiply the number of ticks by the gear ratio of the cartridge, and then by 360 to get the
        // value in degrees
        case pros::MotorGears::blue: return from_stDeg(360 * (counts / 300.0));
        case pros::MotorGears::green: return from_stDeg(360 * (counts / 900.0));
        case pros::MotorGears::red: return from_stDeg(360 * (counts / 1800.0));
        default: return from_stDeg(INFINITY);
    }
}

int Motor::setAngle(Angle angle) {
    const int raw = m_motor.get_raw_position(NULL);
    const Cartridge cartridge = getCartridge();
    // check for errors
    if (raw == INT_MAX) return INT_MAX;
    if (cartridge == Cartridge::INVALID) return INT_MAX;
    // calculate ticks per rotation
    const double tpr = (50.0 * 3600.0 / static_cast<int>(cartridge));
    const double position = raw / tpr;
    // handle different units
    switch (m_motor.get_encoder_units()) {
        case (pros::MotorUnits::degrees):
            return convertStatus(m_motor.set_zero_position(to_stDeg(angle) - position * 360.0));
        case (pros::MotorUnits::rotations): return convertStatus(m_motor.set_zero_position(to_stRot(angle) - position));
        case (pros::MotorUnits::counts): return convertStatus(m_motor.set_zero_position(to_stRot(angle) * tpr - raw));
        default: return INT_MAX;
    }
}

MotorType Motor::getType() {
    // there is no exposed api to get the motor type
    // while the memory address of the function has been found through reverse engineering,
    // it may break between VEXos updates. Instead, we see if we can change the cartridge to something other
    // than the green cartridge, which is only possible on the V5 motor
    const pros::MotorGears oldCart = m_motor.get_gearing();
    const int result = m_motor.set_gearing(pros::v5::MotorGears::red);
    // check for errors
    if (oldCart == pros::MotorGears::invalid) return MotorType::INVALID;
    if (result == INT_MAX) return MotorType::INVALID;
    // check if the gearing changed or not
    const pros::MotorGears newCart = m_motor.get_gearing();
    if (newCart == pros::v5::MotorGears::invalid) return MotorType::INVALID;
    if (newCart != pros::v5::MotorGears::green) {
        // set the cartridge back to its original value
        if (m_motor.set_gearing(oldCart) == INT_MAX) return MotorType::INVALID;
        else return MotorType::V5;
    } else return MotorType::EXP;
}

Cartridge Motor::getCartridge() const {
    const pros::MotorGears cartridge = m_motor.get_gearing();
    // convert the cartridge to our enum
    switch (cartridge) {
        case pros::MotorGears::blue: return Cartridge::BLUE;
        case pros::MotorGears::green: return Cartridge::GREEN;
        case pros::MotorGears::red: return Cartridge::RED;
        default: return Cartridge::INVALID;
    }
}

bool Motor::isReversed() const {
    // technically this returns an int, but as long as you only pass 0 to the index its impossible for it to return an
    // error. This is because PROS stores whether a motor is reversed in the Motor object instead of the Motor hardware.
    return m_motor.is_reversed();
}

void Motor::setReversed(bool reversed) {
    // technically this returns an int, but as long as you only pass 0 to the index its impossible for it to return an
    // error. This is because PROS stores whether a motor is reversed in the Motor object instead of the Motor hardware.
    m_motor.set_reversed(reversed);
}

int Motor::getPort() const { return m_motor.get_port(); }

int Motor::getAbsoluteCounts() {
    // get telemetry from the motor
    const Cartridge cartridge = getCartridge();
    const double position = m_motor.get_position();
    // error checking
    if (cartridge == Cartridge::INVALID) return INT_MAX;
    if (position == INFINITY) return INT_MAX;
    // calculate ticks per rotation
    const double tpr = 50 * (3600.0 / static_cast<int>(cartridge));
    // handle different units
    switch (m_motor.get_encoder_units()) {
        case (pros::MotorUnits::degrees): return (position / 360.0) * tpr;
        case (pros::MotorUnits::rotations): return position * tpr;
        case (pros::MotorUnits::counts): return position;
        default: return INT_MAX;
    }
}
} // namespace lemlib