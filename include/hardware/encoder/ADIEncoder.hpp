#pragma once

#include "hardware/encoder/Encoder.hpp"
#include "pros/adi.hpp"

namespace lemlib {
class ADIEncoder : public Encoder {
    public:
        /**
         * @brief Construct a new ADIEncoder object
         *
         * @param topPort the top port of the ADIEncoder sensor
         * @param bottomPort the bottom port of the ADIEncoder sensor
         * @param reversed whether the encoder is reversed or not
         */
        ADIEncoder(char topPort, char bottomPort, bool reversed = false);
        /**
         * @brief Construct a new ADIEncoder object
         *
         * @param port the ports of the ADIEncoder along with the port of the ADI expander it is connected to
         * @param reversed
         */
        ADIEncoder(pros::adi::ext_adi_port_tuple_t port, bool reversed = false);
        /**
         * @brief Construct a new ADIEncoder object
         *
         * @param ADIEncoder the pros::ADIEncoder object to use
         */
        ADIEncoder(pros::adi::Encoder ADIEncoder);
        /**
         * @brief calibrate the encoder
         *
         * This function calibrates the encoder. The V5 ADIEncoder sensor does not actually need to be calibrated, so
         * this function does nothing
         *
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<void, EncoderError> calibrate() override;
        /**
         * @brief whether the encoder has been calibrated
         *
         * Since the V5 ADIEncoder sensor does not need to be calibrated, this function just checks if the sensor is
         * connected or not
         *
         * @return tl::expected<bool, EncoderError>
         */
        tl::expected<bool, EncoderError> isCalibrated() override;
        /**
         * @brief whether the encoder is calibrating
         *
         * @return tl::expected<bool, EncoderError>
         */
        tl::expected<bool, EncoderError> isCalibrating() override;
        /**
         * @brief whether the encoder is connected
         *
         * @return tl::expected<bool, EncoderError>
         */
        tl::expected<bool, EncoderError> isConnected() override;
        /**
         * @brief Get the absolute angle of the encoder
         *
         * This function sets the absolute angle of the encoder. The ADIEncoder encoder does not measure absolute angle,
         * so this function just sets the relative angle instead
         *
         * @return tl::expected<Angle, EncoderError>
         */
        tl::expected<Angle, EncoderError> getAbsoluteAngle() override;
        /**
         * @brief Set the relative angle of the encoder
         *
         * This function sets the relative angle of the encoder. The relative angle is the number of rotations the
         * encoder has measured since the last reset.
         *
         * @param angle
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<Angle, EncoderError> getRelativeAngle() override;
        /**
         * @brief Set the absolute angle of the encoder
         *
         * Since the ADIEncoder can't set its measured value to a specific value, calling this function just sets the
         * measured value to zero. This function is non-blocking.
         *
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<void, EncoderError> setAbsoluteAngle(Angle) override;
        /**
         * @brief Set the relative angle of the encoder
         *
         * Since the ADIEncoder can't set its measured value to a specific value, calling this function just sets the
         * measured value to zero. This function is non-blocking.
         *
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<void, EncoderError> setRelativeAngle(Angle) override;
        /**
         * @brief Set whether the encoder is reversed or not
         *
         * @param reversed
         * @return tl::expected<void, EncoderError>
         */
        tl::expected<void, EncoderError> setReversed(bool reversed) override;
        /**
         * @brief Get whether the encoder is reversed or not
         *
         * this function function is not implemented for the ADIEncoder due to Vex SDK limitations
         *
         * @return tl::expected<bool, EncoderError>
         */
        tl::expected<bool, EncoderError> isReversed() override;
        /**
         * @brief Get the port of the encoder
         *
         * @return tl::expected<pros::adi::ext_adi_port_tuple_t, EncoderError>
         */
        tl::expected<pros::adi::ext_adi_port_tuple_t, EncoderError> getPort();
    protected:
        pros::adi::Encoder encoder;
};
} // namespace lemlib