#include "hardware/Imu/V5Imu.hpp"

namespace lemlib {
V5Imu::V5Imu(pros::Imu imu)
    : m_imu(imu) {}

int V5Imu::calibrate() { return m_imu.reset(); }

int V5Imu::isCalibrated() { return m_imu.is_calibrating(); }

int V5Imu::isCalibrating() { return m_imu.is_calibrating(); }

int V5Imu::isConnected() { return m_imu.is_installed(); }

Angle V5Imu::getRotation() {
    const double result = m_imu.get_rotation();
    // check for errors
    if (result == INFINITY) return from_stDeg(INFINITY);
    return from_cDeg(result);
}

int V5Imu::setRotation(Angle rotation) { return m_imu.set_rotation(to_cDeg(rotation)); }
} // namespace lemlib