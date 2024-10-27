#include "main.h"
#include "hardware/Motor/MotorGroup.hpp"

void initialize() {
    pros::Motor motorA = pros::Motor(8);
    pros::Motor motorB = pros::Motor(9);
    lemlib::MotorGroup group({motorA, motorB}, 200_rpm);
    pros::lcd::initialize();
    pros::Task screen_task([&]() {
        Angle prev = 0_stDeg;
        while (true) {
            const Angle cur = group.getAngle();
            const AngularVelocity vel = (cur - prev) / 20_msec;
            prev = cur;
            pros::lcd::print(0, "rps: %f", to_rps(vel));
            std::string mode;
            switch (group.getBrakeMode()) {
                case lemlib::BrakeMode::COAST: {
                    mode = "coast";
                    break;
                }
                case lemlib::BrakeMode::BRAKE: {
                    mode = "brake";
                    break;
                }
                case lemlib::BrakeMode::HOLD: {
                    mode = "hold";
                    break;
                }
                case lemlib::BrakeMode::INVALID: {
                    mode = "invalid";
                    break;
                }
            }
            pros::lcd::print(1, "mode: %s", mode.c_str());
            pros::delay(20);
        }
    });
    while (true) {
        double speed;
        std::cin >> speed;
        if (speed == 0) {
            group.setBrakeMode(lemlib::BrakeMode::COAST);
            group.brake();
        } else if (speed == 1) {
            group.setBrakeMode(lemlib::BrakeMode::BRAKE);
            group.brake();
        } else if (speed == -1) {
            group.setBrakeMode(lemlib::BrakeMode::HOLD);
            group.brake();
        } else {
            group.moveVelocity(from_rps(speed));
        }
        pros::delay(10);
    }
}