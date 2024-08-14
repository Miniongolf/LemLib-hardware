#pragma once

#include "hardware/IMU/IMU.hpp"
#include "pros/imu.hpp"

namespace lemlib {
class V5IMU : public IMU {
    public:
        V5IMU(pros::Imu imu);
        int calibrate() override;
        int isCalibrated() override;
        int isCalibrating() override;
        bool isConnected() override;
        Angle getRotation() override;
        int setRotation(Angle rotation) override;
    private:
        pros::Imu m_imu;
};
} // namespace lemlib