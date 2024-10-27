#include "main.h"
#include "hardware/Motor/Motor.hpp"
#include "units/Angle.hpp"

void initialize() {
    lemlib::Motor motor = pros::Motor(8);
    pros::lcd::initialize();
    pros::Task screen_task([&]() {
        while (true) {
            const Angle offset = motor.getOffset();
            const Angle pos = motor.getAngle();
            pros::lcd::print(0, "offset: %f", to_stDeg(offset));
            pros::lcd::print(1, "pos: %f", to_stDeg(pos));
            pros::delay(20);
        }
    });
    while (true) {
        double offset;
        std::cin >> offset;
        motor.setOffset(from_stDeg(offset));
        pros::delay(10);
    }
}