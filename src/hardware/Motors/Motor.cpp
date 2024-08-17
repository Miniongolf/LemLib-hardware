#include "hardware/Motors/Motor.hpp"
#include "pros/abstract_motor.hpp"
#include "units/Angle.hpp"
#include <climits>

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
    if (getType() == MotorType::V5) {
        return m_motor.move_voltage(percent * 12000);
    } else {
        return m_motor.move_voltage(percent * 7200);
    }
}

int Motor::moveVelocity(AngularVelocity velocity) {
    // pros uses an integer value to represent the rpm of the motor
    return m_motor.move_velocity(to_rpm(units::round(velocity, rpm)));
}

int Motor::brake() { return m_motor.brake(); }

int Motor::setBrakeMode(BrakeMode mode) { return m_motor.set_brake_mode(brakeModeToMotorBrake(mode)); }

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
    const pros::MotorUnits units = m_motor.get_encoder_units();
    const Cartridge cartridge = getCartridge();
    // check for errors
    if (raw == INT_MAX) return INT_MAX;
    if (cartridge == Cartridge::INVALID) return INT_MAX;
    // calculate ticks per rotation
    const double tpr = (50 * 3600.0 / static_cast<int>(cartridge));
    const double position = raw / tpr;
    // handle different units
    switch (units) {
        case (pros::MotorUnits::degrees): return !m_motor.set_zero_position(to_stDeg(angle) - position * 360);
        case (pros::MotorUnits::rotations): return !m_motor.set_zero_position(to_stRot(angle) - position);
        case (pros::MotorUnits::counts): return !m_motor.set_zero_position(to_stRot(angle) * tpr - raw);
        default: return INT_MAX;
    }
}

MotorType Motor::getType() {
    // there is no exposed api to get the motor type
    // while the memory address of the function has been found through reverse engineering,
    // it may break between VEXos updates. Instead, we see if we can change the cartridge to something other
    // than the green cartridge, which is only possible on the V5 motor
    const pros::MotorGears oldCart = m_motor.get_gearing();
    // check for errors
    if (oldCart == pros::MotorGears::invalid) return MotorType::INVALID;
    m_motor.set_gearing(pros::v5::MotorGears::red);
    if (m_motor.get_gearing() == pros::v5::MotorGears::red) {
        m_motor.set_gearing(oldCart);
        return MotorType::V5;
    } else return MotorType::EXP;
}

Cartridge Motor::getCartridge() const {
    const pros::MotorGears cartridge = m_motor.get_gearing();
    // check for errors
    if (cartridge == pros::MotorGears::invalid) return Cartridge::INVALID;
    // convert the cartridge to our enum
    switch (cartridge) {
        case pros::MotorGears::blue: return Cartridge::BLUE;
        case pros::MotorGears::green: return Cartridge::GREEN;
        case pros::MotorGears::red: return Cartridge::RED;
        default: return Cartridge::INVALID;
    }
}

bool Motor::isReversed() const {
    // technically this returns an int, because if a non-zero value is passed to pros::Motor::is_reversed(), it will
    // return INT_MAX
    // but as long as we don't (which is easy, just don't pass a value to it), it will either return a 0 or 1
    return m_motor.is_reversed();
}

void Motor::setReversed(bool reversed) { m_motor.set_reversed(reversed); }

int Motor::getPort() const { return m_motor.get_port(); }

int Motor::getAbsoluteCounts() {
    // get telemetry from the motor
    const Cartridge cartridge = getCartridge();
    const pros::MotorUnits units = m_motor.get_encoder_units();
    const double position = m_motor.get_position();
    // error checking
    if (cartridge == Cartridge::INVALID) return INT_MAX;
    if (position == INFINITY) return INT_MAX;
    // calculate ticks per rotation
    const double tpr = 50 * (3600.0 / static_cast<int>(cartridge));
    // handle different units
    switch (units) {
        case (pros::MotorUnits::degrees): return (position / 360.0) * tpr;
        case (pros::MotorUnits::rotations): return position * tpr;
        case (pros::MotorUnits::counts): return position;
        default: return INT_MAX;
    }
}
} // namespace lemlib