#pragma once

#include "units/Angle.hpp"
#include "units/Vector3D.hpp"

namespace lemlib {
class IMU {
    public:
        /**
         * @brief calibrate the IMU
         *
         * This function calibrates the IMU. Usually its necessary to calibrate an IMU before using it.
         * This function is non-blocking.
         */
        virtual void calibrate() = 0;
        /**
         * @brief check if the IMU is calibrated
         *
         * @return true the IMU is calibrated
         * @return false the IMU is not calibrated
         */
        virtual bool isCalibrated() = 0;
        /**
         * @brief check if the IMU is calibrating
         *
         * @return true the IMU is calibrating
         * @return false the IMU is not calibrating
         */
        virtual bool isCalibrating() = 0;
        /**
         * @brief whether the IMU is connected
         *
         * @return true the IMU is connect
         * @return false the IMU is not connected
         */
        virtual bool isConnected() = 0;
        /**
         * @brief Get the rotation of the IMU
         *
         * This function returns the unbounded heading of the IMU
         *
         * @return Angle
         */
        virtual Angle getRotation() = 0;
        /**
         * @brief Set the rotation of the IMU
         *
         * This function sets the unbounded heading of the IMU
         *
         * @param rotation
         */
        virtual void setRotation(Angle rotation) = 0;
        /**
         * @brief Get the yaw measured by the IMU
         *
         * @return Angle
         */
        virtual Angle getYaw() = 0;
        /**
         * @brief Get the pitch measured by the IMU
         *
         * @return Angle
         */
        virtual Angle getPitch() = 0;
        /**
         * @brief Get the roll measured by the IMU
         *
         * @return Angle
         */
        virtual Angle getRoll() = 0;
        /**
         * @brief Set the yaw measured by the IMU
         *
         * @param yaw
         */
        virtual void setYaw(Angle yaw) = 0;
        /**
         * @brief Set the pitch measured by the IMU
         *
         * @param pitch
         */
        virtual void setPitch(Angle pitch) = 0;
        /**
         * @brief Set the roll measured by the IMU
         *
         * @param roll
         */
        virtual void setRoll(Angle roll) = 0;
        /**
         * @brief Get the orientation of the IMU in 3D space
         *
         * @return units::Vector3D<Angle>
         */
        virtual units::Vector3D<Angle> getOrientation() = 0;
        /**
         * @brief Get the Linear Acceleration measured by the IMU in 3D space
         *
         * @return units::Vector3D<LinearAcceleration>
         */
        virtual units::Vector3D<LinearAcceleration> getAcceleration() = 0;
        /**
         * @brief Get the Angular Velocity measured by the IMU in 3D space
         *
         * @return units::Vector3D<AngularVelocity>
         */
        virtual units::Vector3D<AngularVelocity> getAngularVelocity() = 0;
        virtual ~IMU() = default;
};
} // namespace lemlib
