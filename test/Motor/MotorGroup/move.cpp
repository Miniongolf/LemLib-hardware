#include "main.h"
#include "hardware/Motor/MotorGroup.hpp"
#include "units/Angle.hpp"

void initialize() {
    pros::Motor motorA(8);
    pros::Motor motorB(9);
    lemlib::MotorGroup group({motorA, motorB}, 200_rpm);
    pros::lcd::initialize();
    pros::Task screen_task([&]() {
        Angle prev = 0_stDeg;
        while (true) {
            const Angle cur = group.getAngle();
            const AngularVelocity vel = (cur - prev) / 20_msec;
            prev = cur;
            pros::lcd::print(0, "vel: %f", to_rps(vel));
            pros::lcd::print(1, "pos: %f", to_stDeg(cur));
            pros::delay(20);
        }
    });
    while (true) {
        double speed;
        std::cin >> speed;
        group.move(speed);
        pros::delay(10);
    }
    while (true) pros::delay(10);
}