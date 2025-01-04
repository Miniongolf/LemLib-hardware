#include "main.h"
#include "hardware/Motor/Motor.hpp"
#include "hardware/Motor/MotorGroup.hpp"

void initialize() {
    auto motor = lemlib::Motor(1, 200_rpm);
    pros::delay(3000);
    std::cout << sizeof(int) << std::endl;
}