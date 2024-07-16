#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "units/Angle.hpp"
#include "units/Temperature.hpp"

namespace lemlib {

enum BrakeMode { COAST = 0, BRAKE = 1, HOLD = 2 };

enum OverTempLevel { NONE = 0, LEVEL_1 = 1, LEVEL_2 = 2, LEVEL_3 = 3, LEVEL_4 = 4 };

class AbstractMotor : public Encoder {
    public:
        virtual int movePercent(double percent) = 0;
        virtual int moveVoltage(Voltage voltage) = 0;
        virtual int moveVelocity(AngularVelocity velocity) = 0;
        virtual int moveToAngle(Angle angle) = 0;
        virtual int moveByAngle(Angle angle) = 0;
        virtual int brake() = 0;
        virtual int setBrakeMode(BrakeMode mode) = 0;
        virtual int setProfiledVelocity(AngularVelocity velocity) = 0;
        virtual Angle getTargetAngle() = 0;
        virtual BrakeMode getBrakeMode() = 0;
        virtual Power getPower() = 0;
        virtual double getEfficiency() = 0;
        virtual Temperature getTemperature() = 0;
        virtual Torque getTorque() = 0;
        virtual OverTempLevel getOverTemperature() = 0;
        virtual int isOverCurrent() = 0;
        virtual Current getCurrent() = 0;
        virtual Voltage getVoltage() = 0;
        virtual Current getCurrentLimit() = 0;
        virtual Voltage getVoltageLimit() = 0;
        virtual int setCurrentLimit(Current current) = 0;
        virtual int setVoltageLimit(Voltage voltage) = 0;
};
} // namespace lemlib