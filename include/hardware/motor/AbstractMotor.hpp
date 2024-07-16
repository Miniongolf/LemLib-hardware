#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "units/Angle.hpp"
#include "units/Temperature.hpp"

namespace lemlib {

enum BrakeMode { COAST = 0, BRAKE = 1, HOLD = 2 };

enum OverTempLevel { NONE = 0, LEVEL_1 = 1, LEVEL_2 = 2, LEVEL_3 = 3, LEVEL_4 = 4 };

class AbstractMotor : public Encoder {
    public:
        virtual bool movePercent(double percent) = 0;
        virtual bool moveVoltage(Voltage voltage) = 0;
        virtual bool moveVelocity(AngularVelocity velocity) = 0;
        virtual bool moveToAngle(Angle angle) = 0;
        virtual bool moveByAngle(Angle angle) = 0;
        virtual bool brake() = 0;
        virtual bool setBrakeMode(BrakeMode mode) = 0;
        virtual bool setProfiledVelocity(AngularVelocity velocity) = 0;
        virtual Angle getTargetAngle() = 0;
        virtual AngularVelocity getTargetVelocity() = 0;
        virtual Current getCurrent() = 0;
        virtual Voltage getVoltage() = 0;
        virtual Power getPower() = 0;
        virtual double getEfficiency() = 0;
        virtual Temperature getTemperature() = 0;
        virtual Torque getTorque() = 0;
        virtual OverTempLevel getOverTemperature() = 0;
        virtual int isOverCurrent() = 0;
        virtual BrakeMode getBrakeMode() = 0;
        virtual Current getCurrentLimit() = 0;
        virtual Voltage getVoltageLimit() = 0;
        virtual bool setCurrentLimit(Current current) = 0;
        virtual bool setVoltageLimit(Voltage voltage) = 0;
};
} // namespace lemlib