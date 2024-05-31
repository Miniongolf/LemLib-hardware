#pragma once

namespace lemlib {
class Gyro {
    public:
        virtual void calibrate() = 0;
        virtual ~Gyro();
};
} // namespace lemlib