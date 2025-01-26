#ifndef SOFTWARE_MODEM_H
#define SOFTWARE_MODEM_H

#include <vector>
#include <cstdint>

namespace swmodem {

    /// SoftwareModem implements all basic modem functionalities in software.
    /// It modulates data using FSK (Frequency-Shift Keying) and demodulates
    /// data using the reverse process.
    class SoftwareModem {
    public:
        /// Construct a new SoftwareModem
        /// @param sample_rate modem sampling rate
        /// @param base_freq signal base frequency
        /// @param freq_step signal frequency step
        SoftwareModem(double sample_rate, double base_freq, double freq_step);

        /// Modulate data into a sine wave audio signal
        /// @param data data to modulate as byte array
        /// @return modulated sine wave signal as a vector of double
        [[nodiscard]] std::vector<double> modulate(const std::vector<uint8_t>& data) const;

        /// Demodulate a sine wave signal into data
        /// @param signal sine wave signal as vector of double
        /// @return demodulated sine wave signal as data vector of bytes
        [[nodiscard]] std::vector<uint8_t> demodulate(const std::vector<uint8_t>& signal) const;
    private:
        double sample_rate;     // Samples per second
        double base_freq;       // Base modulation frequency
        double freq_step;       // Frequency step for different bit values

        /// Generate a sine wave signal
        /// @param freq signal frequency
        /// @param duration signal duration
        /// @return generated sine wave signal as vector of double values
        [[nodiscard]] std::vector<double> generate_sine_wave(double freq, double duration) const;
    };

}

#endif