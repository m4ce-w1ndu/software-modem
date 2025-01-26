#include "SoftwareModem.h"

namespace swmodem {

    SoftwareModem::SoftwareModem(const double sample_rate, const double base_freq, const double freq_step)
        : sample_rate(sample_rate), base_freq(base_freq), freq_step(freq_step) {}

    std::vector<double> SoftwareModem::modulate(const std::vector<uint8_t> &data) const
    {
        std::vector<double> audio_signal;

        for (uint8_t byte : data) {
            for (int bit = 0; bit < 8; ++bit) {
                const double bit_duration = 0.01;

                const bool is_one = (byte & (1 << bit)) != 0;
                const double freq = is_one ? base_freq + freq_step : base_freq;

                // Generate sine wave for this bit
                std::vector<double> sine_wave = generate_sine_wave(freq, bit_duration);
                audio_signal.insert(audio_signal.end(), sine_wave.begin(), sine_wave.end());
            }
        }

        return audio_signal;
    }

    std::vector<uint8_t> SoftwareModem::demodulate(const std::vector<double> &signal) const
    {
        std::vector<uint8_t> data;
        constexpr double bit_duration = 0.01;
        const auto samples_per_bit = static_cast<size_t>(sample_rate * bit_duration);

        for (size_t i = 0; i < signal.size(); i += samples_per_bit * 8) {
            uint8_t byte = 0;

            for (int bit = 0; bit < 8; ++bit) {
                const size_t bit_start = i + bit * samples_per_bit;

                if (bit_start + samples_per_bit > signal.size()) {
                    break;
                }

                // Analyze signal for this bit
                double sum = 0.0;
                for (size_t j = 0; j < samples_per_bit; ++j) {
                    sum += signal[bit_start + j];
                }

                const double average = sum / static_cast<double>(samples_per_bit);

                // Threshold to distinguish between 0 and 1
                if (average > 0.5) {
                    byte |= (1 << bit);
                }
            }

            data.push_back(byte);
        }

        return data;
    }

    std::vector<double> SoftwareModem::generate_sine_wave(const double freq, const double duration) const
    {
        const auto total_samples = static_cast<size_t>(sample_rate * duration);
        std::vector<double> sine_wave(total_samples);

        for (size_t i = 0; i < total_samples; ++i) {
            sine_wave[i] = std::sin(2.0 * M_PI * freq * static_cast<double>(i) / sample_rate);
        }

        return sine_wave;
    }

}