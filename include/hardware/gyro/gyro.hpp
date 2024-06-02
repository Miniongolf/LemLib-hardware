#pragma once

namespace lemlib {

enum GyroStatus { CALIBRATED = 0, CALIBRATING = 1, NOT_CALIBRATED = 2, UNKOWN_ERROR = 3 };

class Gyro {
    public:
        virtual void calibrate() = 0;
        virtual int getStatus() = 0;
        virtual ~Gyro();
};
} // namespace lemlib