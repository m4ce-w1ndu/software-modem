#include "SoftwareModem.h"

#include <cmath>

namespace swmodem {

    SoftwareModem::SoftwareModem(const double sample_rate, const double base_freq, const double freq_step, const double bit_duration)
        : sample_rate(sample_rate), base_freq(base_freq), freq_step(freq_step), bit_duration(bit_duration) {}

    std::vector<double> SoftwareModem::modulate(const std::vector<uint8_t> &data) const
    {
        std::vector<double> audio_signal;

        for (const uint8_t byte : data) {
            for (int bit = 0; bit < 8; ++bit) {
                const bool is_one = (byte & (1 << bit)) != 0;
                const double freq = is_one ? base_freq + freq_step : base_freq;

                // Generate sine wave for this bit
                std::vector<double> sine_wave = generate_sine_wave(freq);
                audio_signal.insert(audio_signal.end(), sine_wave.begin(), sine_wave.end());
            }
        }

        return audio_signal;
    }

    std::vector<uint8_t> SoftwareModem::demodulate(const std::vector<double> &signal) const
    {
        std::vector<uint8_t> data;
        const auto samples_per_bit = static_cast<size_t>(sample_rate * bit_duration);

        for (size_t i = 0; i < signal.size(); i += samples_per_bit * 8) {
            uint8_t byte = 0;

            for (int bit = 0; bit < 8; ++bit) {
                const size_t bit_start = i + bit * samples_per_bit;

                if (bit_start + samples_per_bit > signal.size()) {
                    break;
                }

                // Analyse signal for this bit with Goertzel
                const double power_zero = goertzel(signal, bit_start, samples_per_bit, base_freq);
                const double power_one = goertzel(signal, bit_start, samples_per_bit, base_freq + freq_step);

                // Decide the bit value based on stronger power
                if (power_one > power_zero) {
                    byte |= (1 << bit);
                }
            }

            data.push_back(byte);
        }

        return data;
    }

    std::vector<double> SoftwareModem::generate_sine_wave(const double freq) const
    {
        const auto total_samples = static_cast<size_t>(sample_rate * bit_duration);
        std::vector<double> sine_wave(total_samples);

        for (size_t i = 0; i < total_samples; ++i) {
            sine_wave[i] = std::sin(2.0 * M_PI * freq * static_cast<double>(i) / sample_rate);
        }

        return sine_wave;
    }

    double SoftwareModem::goertzel(const std::vector<double> &signal, const size_t start, const size_t samples,
        const double tgt_freq) const
    {
        double s_prev = 0.0;
        double s_prev2 = 0.0;
        const double normalized_freq = 2.0 * M_PI * tgt_freq / sample_rate;

        for (size_t i = 0; i < samples; ++i) {
            const double sample = signal[start + i];
            const double s = sample + 2.0 * std::cos(normalized_freq) * s_prev - s_prev2;
            s_prev2 = s_prev;
            s_prev = s;
        }

        // Power at the target frequency
        const double power = s_prev2 * s_prev2 + s_prev * s_prev - 2.0 * std::cos(normalized_freq) * s_prev * s_prev2;
        return power;
    }
}
