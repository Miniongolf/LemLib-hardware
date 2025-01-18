#include "hardware/IMU/V5InertialSensor.hpp"
#include "hardware/Port.hpp"
#include "pros/imu.hpp"
#include <mutex>

namespace lemlib {
V5InertialSensor::V5InertialSensor(SmartPort port)
    : m_imu(port) {}

V5InertialSensor::V5InertialSensor(V5InertialSensor& other)
    : m_imu(other.m_imu),
      m_offset(other.m_offset) {}

V5InertialSensor V5InertialSensor::from_pros_imu(pros::IMU imu) {
    return V5InertialSensor {{imu.get_port(), runtime_check_port}};
}

int V5InertialSensor::calibrate() {
    std::lock_guard lock(m_mutex);
    m_offset = 0_stRot;
    return m_imu.reset();
}

int V5InertialSensor::isCalibrated() { return m_imu.is_calibrating(); }

int V5InertialSensor::isCalibrating() { return m_imu.is_calibrating(); }

int V5InertialSensor::isConnected() { return m_imu.is_installed(); }

Angle V5InertialSensor::getRotation() {
    std::lock_guard lock(m_mutex);
    const double result = m_imu.get_rotation();
    // check for errors
    if (result == INFINITY) return from_stDeg(INFINITY);
    return from_cDeg(result * m_gyroScalar) + m_offset;
}

int V5InertialSensor::setRotation(Angle rotation) {
    std::lock_guard lock(m_mutex);
    Angle raw = this->getRotation();
    if (to_stRot(raw) == INFINITY) return INT32_MAX;
    else {
        m_offset = rotation - raw;
        return 0;
    }
}

int V5InertialSensor::setGyroScalar(Number scalar) {
    std::lock_guard lock(m_mutex);
    m_gyroScalar = scalar;
    return 0;
}

Number V5InertialSensor::getGyroScalar() {
    std::lock_guard lock(m_mutex);
    return m_gyroScalar;
}
} // namespace lemlib