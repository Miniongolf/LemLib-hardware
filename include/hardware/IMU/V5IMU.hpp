#pragma once

#include "hardware/IMU/IMU.hpp"
#include "pros/imu.hpp"

namespace lemlib {
class V5IMU : public IMU {
    public:
        /**
         * @brief Construct a new V5IMU object
         *
         * @param imu the pros::Imu object
         */
        V5IMU(pros::Imu imu);
        /**
         * @brief calibrate the V5 Inertial Sensor
         *
         * This function calibrates the V5 Inertial Sensor. Its necessary to call this function before use.
         * This function is non-blocking.
         */
        void calibrate() override;
        /**
         * @brief check if the IMU is calibrated
         *
         * @return true the IMU is calibrated
         * @return false the IMU is not calibrated
         */
        bool isCalibrated() override;
        /**
         * @brief check if the IMU is calibrating
         *
         * @return true the IMU is calibrating
         * @return false the IMU is not calibrating
         */
        bool isCalibrating() override;
        /**
         * @brief whether the IMU is connected
         *
         * @return true the IMU is connect
         * @return false the IMU is not connected
         */
        bool isConnected() override;
        /**
         * @brief Get the rotation of the IMU
         *
         * This function returns the unbounded heading of the IMU
         *
         * @return Angle
         */
        Angle getRotation() override;
        /**
         * @brief Set the rotation of the IMU
         *
         * This function sets the unbounded heading of the IMU
         *
         * @param rotation
         */
        void setRotation(Angle rotation) override;
    private:
        pros::Imu m_imu;
};
} // namespace lemlib