#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "units/Angle.hpp"
#include "units/Temperature.hpp"

namespace lemlib {

enum BrakeMode { COAST = 0, BRAKE = 1, HOLD = 2 };

class AbstractMotor : public Encoder {
    public:
        virtual int move(double percent) = 0;
        virtual int moveVelocity(AngularVelocity velocity) = 0;
        virtual int brake() = 0;
        virtual int setBrakeMode(BrakeMode mode) = 0;
        virtual BrakeMode getBrakeMode() = 0;
        virtual Temperature getTemperature() = 0;
};
} // namespace lemlib