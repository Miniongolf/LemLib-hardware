#pragma once

#include "units/Angle.hpp"

namespace lemlib {

class IMU {
    public:
        virtual void calibrate() = 0;
        virtual void reset() = 0;
        virtual Angle getHeading() = 0;
        virtual Angle getOrientation() = 0;
        virtual Angle getYaw() = 0;
        virtual Angle getPitch() = 0;
        virtual Angle getRoll() = 0;
        virtual ~IMU() = default;
};
} // namespace lemlib
