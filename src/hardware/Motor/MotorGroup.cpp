#include "hardware/Motor/MotorGroup.hpp"
#include "Motor.hpp"
#include "units/Temperature.hpp"
#include <climits>
#include <cmath>
#include <errno.h>

namespace lemlib {
MotorGroup::MotorGroup(std::initializer_list<int> ports, AngularVelocity outputVelocity)
: m_outputVelocity(outputVelocity) {
    for (const int port : ports) {
        m_motors.push_back({.port = port, .connectedLastCycle = true, .offset = 0_stDeg});
    }
}

int MotorGroup::move(double percent) {
    const std::vector<Motor> motors = getMotors();
    bool success = false;
    for (Motor motor : motors) {
        const int result = motor.move(percent);
        if (result == 0) success = true;
    }
    // as long as one motor moves successfully, return 0 (success)
    return success ? 0 : INT_MAX;
}

int MotorGroup::moveVelocity(AngularVelocity velocity) {
    const std::vector<Motor> motors = getMotors();
    bool success = false;
    for (Motor motor : motors) {
        // since the motors in the group are geared together, we need to account for different gearings
        // of different motors in the group
        const Cartridge cartridge = motor.getCartridge();
        // check for errors
        if (cartridge == Cartridge::INVALID) continue;
        // calculate gear ratio
        const Number ratio = from_rpm(static_cast<int>(cartridge)) / m_outputVelocity;
        const int result = motor.moveVelocity(velocity * ratio);
        if (result == 0) success = true;
    }
    // as long as one motor moves successfully, return 0 (success)
    return success ? 0 : INT_MAX;
}

int MotorGroup::brake() {
    const std::vector<Motor> motors = getMotors();
    bool success = false;
    for (Motor motor : motors) {
        const int result = motor.brake();
        if (result == 0) success = true;
    }
    // as long as one motor brakes successfully, return 0 (success)
    return success ? 0 : INT_MAX;
}

int MotorGroup::setBrakeMode(BrakeMode mode) {
    m_brakeMode = mode;
    getMotors(); // even though we don't use this, we call it anyway for brake mode setting and disconnect handling
    return 0;
}

BrakeMode MotorGroup::getBrakeMode() {
    getMotors(); // even though we don't use this, we call it anyway for brake mode setting and disconnect handling
    return m_brakeMode;
}

int MotorGroup::isConnected() {
    const std::vector<Motor> motors = getMotors();
    for (Motor motor : motors) {
        const int result = motor.isConnected();
        if (result == 1) return true;
    }
    // if no motors are connected, return 0
    return 0;
}

Angle MotorGroup::getAngle() {
    const std::vector<Motor> motors = getMotors();
    // get the average angle of all motors in the group
    Angle angle = 0_stDeg;
    int errors = 0;
    for (Motor motor : motors) {
        // get angle
        const Angle result = motor.getAngle();
        if (result == from_stDeg(INFINITY)) {
            errors++;
            continue;
        }; // check for errors
        // get motor cartridge
        const Cartridge cartridge = motor.getCartridge();
        if (cartridge == Cartridge::INVALID) {
            errors++;
            continue;
        }
        // calculate the gear ratio
        const Number ratio = m_outputVelocity / from_rpm(static_cast<int>(cartridge));
        angle += result * ratio;
    }
    // if no motors are connected, return INFINITY
    if (errors == motors.size()) return from_stDeg(INFINITY);
    // otherwise, return the average angle
    return angle / (getSize() - errors);
}

int MotorGroup::setAngle(Angle angle) {
    const std::vector<Motor> motors = getMotors();
    bool success = false;
    for (Motor motor : motors) {
        // since the motors in the group are geared together, we need to account for different gearings
        // of different motors in the group
        const Cartridge cartridge = motor.getCartridge();
        // check for errors
        if (cartridge == Cartridge::INVALID) continue;
        // calculate gear ratio
        const Number ratio = from_rpm(static_cast<int>(cartridge)) / m_outputVelocity;
        const int result = motor.setAngle(angle * ratio);
        if (result == 0) success = true;
    }
    // as long as one motor sets the angle successfully, return 0 (success)
    return success ? 0 : INT_MAX;
}

std::vector<Temperature> MotorGroup::getTemperatures() {
    std::vector<Motor> motors = getMotors();
    std::vector<Temperature> temperatures;
    for (const Motor motor : motors) { temperatures.push_back(motor.getTemperature()); }
    return temperatures;
}

int MotorGroup::getSize() {
    const std::vector<Motor> motors = getMotors();
    int size = 0;
    for (Motor motor : motors)
        if (motor.isConnected()) size++;
    return size;
}

int MotorGroup::addMotor(int port) {
    // check that the motor isn't already part of the group
    for (const MotorInfo& info : m_motors) {
        // return an error if the motor is already added to the group
        if (std::abs(info.port) == std::abs(port)) {
            errno = EEXIST;
            return INT_MAX;
        }
    }
    // configure the motor
    const Angle offset = configureMotor(port);
    // add the motor to the group
    MotorInfo info {.port = port, .connectedLastCycle = offset == from_stRot(INFINITY), .offset = offset};
    m_motors.push_back(info);
    if (offset == from_stRot(INFINITY)) return INT_MAX;
    return 0;
}

int MotorGroup::addMotor(Motor motor) { return addMotor(motor.getPort()); }

int MotorGroup::addMotor(Motor motor, bool reversed) {
    // set the motor reversal
    motor.setReversed(reversed);
    return addMotor(motor);
}

void MotorGroup::removeMotor(int port) {
    // remove the motor with the specified port
    auto it = m_motors.begin();
    while (it < m_motors.end()) {
        if (std::abs(it->port) == std::abs(port)) {
            m_motors.erase(it);
        } else {
            it++;
        }
    }
}

const std::vector<Motor> MotorGroup::getMotors() {
    std::vector<Motor> motors;
    for (int i = 0; i < m_motors.size(); i++) {
        // create a temporary motor
        Motor motor(m_motors.at(i).port);
        // set motor offset
        motor.setOffset(m_motors.at(i).offset);
        // check if the motor is connected
        const bool connected = motor.isConnected();
        // don't add the motor if it is not connected
        if (!connected) {
            m_motors.at(i).connectedLastCycle = false;
            continue;
        }
        // if the motor is connected, but wasn't the last time we checked, then configure it to prevent side
        // effects of reconnecting
        // don't add the motor if configuration fails
        if (!m_motors.at(i).connectedLastCycle) {
            if (std::isinf(to_stRot(configureMotor(m_motors.at(i).port)))) continue;
        }
        // check that the brake mode of the motor is correct
        BrakeMode mode = motor.getBrakeMode();
        if (mode != m_brakeMode) {
            if (motor.setBrakeMode(m_brakeMode) != 0) continue;
        } else if (mode == BrakeMode::INVALID) continue;
        // add the motor and set save it as connected
        m_motors.at(i).connectedLastCycle = true;
        motors.push_back(motor);
    }
    return motors;
}

void MotorGroup::removeMotor(Motor motor) { removeMotor(motor.getPort()); }

Angle MotorGroup::configureMotor(int port) {
    // since this function is called in other MotorGroup member functions, this function can't call any other member
    // function, otherwise it would cause a recursion loop. This means that this function is ugly and complex, but at
    // least it means that the other functions can stay simple

    // this function does not return immediately when something goes wrong. Instead it will continue with the process to
    // add the motor, and the motor will automatically be reconfigured when it is working properly again
    // whether there was a failure or not is kept track of with this boolean
    bool success = true;
    Motor motor(port);
    // set the motor's brake mode to whatever the first working motor's brake mode is
    for (MotorInfo info : m_motors) {
        Motor m = info.port;
        const BrakeMode mode = m.getBrakeMode();
        if (mode == BrakeMode::INVALID) continue;
        if (m.setBrakeMode(mode) == 0) break;
        else {
            success = false;
            break;
        }
    }
    // calculate the angle to set the motor to
    const Cartridge cartridge = motor.getCartridge();
    if (cartridge == Cartridge::INVALID) success = false; // check for errors

    Angle angle = 0_stDeg;
    {
        // find all the working motors
        std::vector<Motor> motors;
        for (int i = 0; i < m_motors.size(); i++) {
            // temporary motor object
            Motor m = m_motors.at(i).port;
            // set the offset of the motor
            m.setOffset(m_motors.at(i).offset);
            // check if the motor is connected
            const bool connected = m.isConnected();
            // check that the motor is not the motor we are configuring
            if (std::abs(m.getPort()) == std::abs(port)) continue;
            // don't add the motor if it is not connected
            if (!connected) continue;
            else motors.push_back(m);
        }

        // get the average angle of all motors in the group
        Angle tempAngle = 0_stDeg;
        int errors = 0;
        for (Motor m : motors) {
            // get angle
            const Angle result = m.getAngle();
            if (result == from_stDeg(INFINITY)) { // check for errors
                errors++;
                continue;
            }
            // get motor cartridge
            const Cartridge cartridge = m.getCartridge();
            if (cartridge == Cartridge::INVALID) { // check for errors
                errors++;
                continue;
            }
            tempAngle += result;
        }
        // prevent divide by zero if all motors failed
        if (motors.size() != errors) angle = tempAngle / (motors.size() - errors);
    }

    // set the angle of the new motor
    const int result = motor.setAngle(angle);
    if (result == INT_MAX) return from_stRot(INFINITY); // check for errors
    if (success == true) return motor.getOffset();
    return from_stRot(INFINITY);
}
}; // namespace lemlib