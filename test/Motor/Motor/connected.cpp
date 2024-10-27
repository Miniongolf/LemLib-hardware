#include "main.h"
#include "hardware/Motor/Motor.hpp"

void initialize() {
    lemlib::Motor motor = pros::Motor(8);
    pros::lcd::initialize();
    while (true) {
        pros::lcd::print(0, "connected: %d", motor.isConnected());
        pros::delay(10);
    }
}