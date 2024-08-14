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
    const int counts = m_motor.get_raw_position(NULL);
    if (counts == INT_MAX) return from_sDeg(INFINITY);
    switch (cartridge) {
        // without a cartridge, the encoder measures 50 counts per revolution
        // so we multiply the number of ticks by the gear ratio of the cartridge, and then by 360 to get the
        // value in degrees
        case pros::MotorGears::blue: return from_sDeg(counts * 6 * 360);
        case pros::MotorGears::green: return from_sDeg(counts * 18 * 360);
        case pros::MotorGears::red: return from_sDeg(counts * 36 * 360);
        default: return from_sDeg(INFINITY);
    }
}

int Motor::setAngle(Angle angle) {
    // PROS does not let us just set the position, or let us set the zero position using raw encoder ticks
    // one solution is to temporarily set the encoder units, but writing ops open us up to race conditions
    // and other problems which aren't fun to deal with
    // so instead, we account for all the possible set encoder units
    const pros::MotorUnits units = m_motor.get_encoder_units();
    const double raw = m_motor.get_position();
    if (units == pros::MotorUnits::invalid) return INT_MAX;
    if (raw == INFINITY) return INT_MAX;
    switch (units) {
        case pros::MotorUnits::counts: {
            const pros::MotorGears cartridge = m_motor.get_gearing();
            if (cartridge == pros::MotorGears::invalid) return INT_MAX;
            switch (cartridge) {
                // without a cartridge, the encoder measures 50 counts per revolution
                // so we multiply the requested angle in rotations to the number of ticks per rotation
                case pros::MotorGears::blue: return m_motor.set_zero_position(raw - (to_sDeg(angle) / 360) * 300);
                case pros::MotorGears::green: return m_motor.set_zero_position(raw - (to_sDeg(angle) / 360) * 900);
                case pros::MotorGears::red: return m_motor.set_zero_position(raw - (to_sDeg(angle) / 360) * 1800);
                default: return INT_MAX;
            }
        }
        case pros::MotorUnits::degrees: return m_motor.set_zero_position(raw - to_sDeg(angle));
        case pros::MotorUnits::rotations: return m_motor.set_zero_position(raw - to_sDeg(angle) / 360);
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

int Motor::getPort() const {
    // PROS returns the port number as a signed integer, which is negative if the motor is reversed.
    // We return the absolute value of the port number to avoid negative port numbers.
    return std::abs(m_motor.get_port());
}
} // namespace lemlib