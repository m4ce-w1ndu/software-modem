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
        /// @param bit_duration bit duration in seconds
        SoftwareModem(double sample_rate, double base_freq, double freq_step, double bit_duration = DEFAULT_BIT_DURATION);

        /// Modulate data into a sine wave audio signal
        /// @param data data to modulate as byte array
        /// @return modulated sine wave signal as a vector of double
        [[nodiscard]] std::vector<double> modulate(const std::vector<uint8_t>& data) const;

        /// Demodulate a sine wave signal into data
        /// @param signal sine wave signal as vector of double
        /// @return demodulated sine wave signal as data vector of bytes
        [[nodiscard]] std::vector<uint8_t> demodulate(const std::vector<double>& signal) const;

        /// Return the sample rate of the Modem
        /// @return sample rate as double in Hz
        [[nodiscard]] constexpr double get_sample_rate() const { return sample_rate; }

        /// Return the base frequency (0-bit value) of the Modem
        /// @return base frequency as double in Hz
        [[nodiscard]] constexpr double get_base_freq() const { return base_freq; }

        /// Return the frequency step (1-bit value offset) of the Modem
        /// @return frequency step as double in Hz
        [[nodiscard]] constexpr double get_freq_step() const { return freq_step; }

        /// Return current bit duration
        /// @return current bit duration in seconds (double)
        [[nodiscard]] constexpr double get_bit_duration() const { return bit_duration; }

        /// Maximum bit duration
        static constexpr double MAX_BIT_DURATION = 0.1;

        /// Default bit duration
        static constexpr double DEFAULT_BIT_DURATION = 0.01;

        /// Minimum bit duration
        static constexpr double MIN_BIT_DURATION = 0.000454;
    private:
        double sample_rate;     // Samples per second
        double base_freq;       // Base modulation frequency
        double freq_step;       // Frequency step for different bit values
        double bit_duration;    // Bit duration

        /// Generate a sine wave signal
        /// @param freq signal frequency
        /// @return generated sine wave signal as vector of double values
        [[nodiscard]] std::vector<double> generate_sine_wave(double freq) const;

        /// Run Goertzel signal analysis algorithm
        /// @param signal signal data
        /// @param start start of the signal
        /// @param samples number of samples
        /// @param tgt_freq target frequency
        /// @return power at the target frequency
        [[nodiscard]] double goertzel(const std::vector<double>& signal, size_t start, size_t samples, double tgt_freq) const;
    };

}

#endif