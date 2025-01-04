#include "hardware/IMU/V5InertialSensor.hpp"

namespace lemlib {
V5InertialSensor::V5InertialSensor(pros::Imu imu)
    : m_imu(imu) {}

V5InertialSensor::V5InertialSensor(std::uint8_t port)
    : m_imu(port) {}

int V5InertialSensor::calibrate() {
    m_offset = 0_stRot;
    return m_imu.reset();
}

int V5InertialSensor::isCalibrated() { return m_imu.is_calibrating(); }

int V5InertialSensor::isCalibrating() { return m_imu.is_calibrating(); }

int V5InertialSensor::isConnected() { return m_imu.is_installed(); }

Angle V5InertialSensor::getRotation() {
    const double result = m_imu.get_rotation();
    // check for errors
    if (result == INFINITY) return from_stDeg(INFINITY);
    return from_cDeg(result * m_gyroScalar) + m_offset;
}

int V5InertialSensor::setRotation(Angle rotation) {
    Angle raw = this->getRotation();
    if (to_stRot(raw) == INFINITY) return INT32_MAX;
    else {
        m_offset = rotation - raw;
        return 0;
    }
}

int V5InertialSensor::setGyroScalar(double scalar) {
    m_gyroScalar = scalar;
    return 0;
}

double V5InertialSensor::getGyroScalar() { return m_gyroScalar; }
} // namespace lemlib