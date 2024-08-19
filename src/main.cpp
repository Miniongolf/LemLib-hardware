#include "main.h"
#include "hardware/motors/MotorGroup.hpp"

pros::Motor motorA(8, pros::v5::MotorGears::green);
pros::Motor motorB(9, pros::v5::MotorGears::green);

lemlib::Motor lemMotorA = motorA;
lemlib::Motor lemMotorB = motorB;

lemlib::MotorGroup group({motorA, motorB}, 200_rpm);

// initialize function. Runs on program startup
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    // print position to brain screen
    pros::Task screen_task([&]() {
        while (true) {
            pros::lcd::print(1, "Group: %f", to_stDeg(group.getAngle()));
            pros::lcd::print(2, "A: %f", to_stDeg(lemMotorA.getAngle()));
            pros::lcd::print(3, "B: %f", to_stDeg(lemMotorB.getAngle()));
            pros::lcd::print(4, "Time: %d", pros::millis());
            // print robot location to the brain screen
            // delay to save resources
            pros::delay(20);
        }
    });
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
    while (true) {
        double angle;
        std::cin >> angle;
        group.setAngle(from_stDeg(angle));
        pros::delay(10);
    }
}
