#include "main.h"
#include "hardware/Motor/MotorGroup.hpp"

void initialize() {
    pros::Motor motorA = pros::Motor(8);
    pros::Motor motorB = pros::Motor(9);
    lemlib::MotorGroup group({motorA, motorB}, 200_rpm);
    pros::lcd::initialize();
    while (true) {
        pros::lcd::print(0, "connected: %d", group.isConnected());
        pros::delay(10);
    }
}