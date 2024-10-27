#include "main.h"
#include "hardware/Motor/Motor.hpp"
#include "pros/abstract_motor.hpp"

void initialize() {
    pros::Motor prosMotor(8);
    lemlib::Motor motor = pros::Motor(8);
    pros::lcd::initialize();
    pros::Task screen_task([&]() {
        while (true) {
            const Angle pos = motor.getAngle();
            pros::lcd::print(0, "pos: %f", to_stDeg(pos));
            pros::delay(20);
        }
    });
    for (int i = 0;; i++) {
        switch (i % 4) { // output should be equal no matter encoder units
            case 0: {
                prosMotor.set_encoder_units(pros::v5::MotorEncoderUnits::counts);
                break;
            }
            case 1: {
                prosMotor.set_encoder_units(pros::v5::MotorEncoderUnits::deg);
                break;
            }
            case 2: {
                prosMotor.set_encoder_units(pros::v5::MotorEncoderUnits::degrees);
                break;
            }
            case 3: {
                prosMotor.set_encoder_units(pros::v5::MotorEncoderUnits::rotations);
                break;
            }
        }
        prosMotor.set_zero_position(i); // setting motor position thru pros should have no effect
        double pos;
        std::cin >> pos;
        motor.setAngle(from_stDeg(pos));
        pros::delay(10);
    }
}