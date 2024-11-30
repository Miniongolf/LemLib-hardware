#include "hardware/Imu/Imu.hpp"

namespace lemlib {
int Imu::setGyroScalar(double scalar) {
    m_gyroScalar = scalar;
    // Always returns 0 because the scalar setter is not dependent on hardware
    return 0;
}

double Imu::getGyroScalar() {
    // Never returns an error because the scalar getter is not dependent on hardware
    return m_gyroScalar;
}
} // namespace lemlib