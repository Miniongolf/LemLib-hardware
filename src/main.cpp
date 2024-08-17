#include "main.h"
#include "hardware/motors/MotorGroup.hpp"
#include "pros/abstract_motor.hpp"

pros::Motor motorA(8, pros::v5::MotorGears::green, pros::MotorEncoderUnits::counts);
pros::Motor motorB(9, pros::v5::MotorGears::green);

lemlib::Motor lemMotorA = motorA;
lemlib::Motor lemMotorB = motorB;

lemlib::MotorGroup group({motorA}, 200_rpm);

// initialize function. Runs on program startup
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    // print position to brain screen
    pros::Task screen_task([&]() {
        while (true) {
            pros::lcd::print(1, "Pos A: %f", to_stDeg(lemMotorA.getAngle()));
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
        lemMotorA.setAngle(from_stDeg(angle));
        pros::delay(10);
    }
}