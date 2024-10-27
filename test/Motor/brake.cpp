#include "main.h"
#include "hardware/Motor/Motor.hpp"

void initialize() {
    lemlib::Motor motor = pros::Motor(8);
    pros::lcd::initialize();
    pros::Task screen_task([&]() {
        Angle prev = 0_stDeg;
        while (true) {
            const Angle cur = motor.getAngle();
            const AngularVelocity vel = (cur - prev) / 20_msec;
            prev = cur;
            pros::lcd::print(0, "rps: %f", to_rps(vel));
            pros::delay(20);
        }
    });
    while (true) {
        double speed;
        std::cin >> speed;
        if (speed == 0) {
            motor.setBrakeMode(lemlib::BrakeMode::COAST);
            motor.brake();
        } else if (speed == 1) {
            motor.setBrakeMode(lemlib::BrakeMode::BRAKE);
            motor.brake();
        } else if (speed == -1) {
            motor.setBrakeMode(lemlib::BrakeMode::HOLD);
            motor.brake();
        } else {
            motor.moveVelocity(from_rps(speed));
        }
        pros::delay(10);
    }
}