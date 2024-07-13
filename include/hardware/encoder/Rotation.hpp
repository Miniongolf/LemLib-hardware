#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "pros/rotation.hpp"

namespace lemlib {
class Rotation : public Encoder {
    public:
        /**
         * @brief Construct a new Rotation object
         *
         * @param port the signed port of the rotation sensor. If the port is negative, the sensor will be reversed.
         */
        Rotation(int port);
        /**
         * @brief Construct a new Rotation object
         *
         * @param rotation the pros::Rotation object to use
         */
        Rotation(pros::Rotation rotation);
        /**
         * @brief calibrate the encoder
         *
         * This function calibrates the encoder. The V5 Rotation sensor does not actually need to be calibrated, so this
         * function does nothing
         */
        void calibrate() override;
        /**
         * @brief whether the encoder has been calibrated
         *
         * Since the V5 Rotation sensor does not need to be calibrated, this function just checks if the sensor is
         * connected or not
         */
        bool isCalibrated() override;
        bool isCalibrating() override;
        bool isConnected() override;
        Angle getAbsoluteAngle() override;
        Angle getRelativeAngle() override;
        void setAbsoluteAngle(Angle angle) override;
        void setRelativeAngle(Angle angle) override;
        void setReversed(bool reversed) override;
        bool isReversed() override;
        int getPort();
    protected:
        pros::Rotation rotation;
};
} // namespace lemlib