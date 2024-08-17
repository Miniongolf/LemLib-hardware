#include "hardware/IMU/V5IMU.hpp"

namespace lemlib {
V5IMU::V5IMU(pros::Imu imu)
    : m_imu(imu) {}

int V5IMU::calibrate() { return m_imu.reset(); }

int V5IMU::isCalibrated() { return m_imu.is_calibrating(); }

int V5IMU::isCalibrating() { return m_imu.is_calibrating(); }

int V5IMU::isConnected() { return m_imu.is_installed(); }

Angle V5IMU::getRotation() {
    const double result = m_imu.get_rotation();
    // check for errors
    if (result == INFINITY) return from_stDeg(INFINITY);
    return from_cDeg(result);
}

int V5IMU::setRotation(Angle rotation) { return m_imu.set_rotation(to_cDeg(rotation)); }
} // namespace lemlib