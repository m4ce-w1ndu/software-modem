#include <SoftwareModem.h>
#include <ModemAudioPlayer.h>

#include <cstdint>
#include <iostream>

std::vector<uint8_t> string_to_bytes(const std::string& text);
void print_as_string(const std::vector<uint8_t>& bytes);

int main(int argc, char* argv[])
{
    constexpr double SAMPLE_RATE = 44100.0;
    constexpr double BASE_FREQ = 1000.0;
    constexpr double FREQ_STEP = 500.0;

    // Initialize a modem
    const swmodem::SoftwareModem modem(SAMPLE_RATE, BASE_FREQ, FREQ_STEP, swmodem::SoftwareModem::MIN_BIT_DURATION);
    std::string input;

    // Initialize a player
    swmodem::ModemAudioPlayer player;

    // Sample data
    std::cout << "input string to encode: ";
    std::getline(std::cin, input);

    const std::vector<uint8_t> data = string_to_bytes(input);

    // Modulate data
    const std::vector<double> audio_data = modem.modulate(data);
    std::cout << "Generated " << audio_data.size() << " audio samples.\n";

    // Play audio
    player.play(audio_data, SAMPLE_RATE);

    // Demodulate audio back into data
    const std::vector<uint8_t> decoded_data = modem.demodulate(audio_data);
    std::cout << "Decoded data:\n";

    for (const uint8_t byte : decoded_data) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << "\n";
    }

    std::cout << "As text: \n";
    print_as_string(decoded_data);

    return 0;
}

std::vector<uint8_t> string_to_bytes(const std::string& text)
{
    std::vector<uint8_t> bytes(text.size());
    std::transform(text.cbegin(), text.cend(), bytes.begin(), [](const char c) {
        return static_cast<uint8_t>(c);
    });
    return bytes;
}

void print_as_string(const std::vector<uint8_t>& bytes)
{
    for (const uint8_t byte : bytes) {
        std::cout << static_cast<char>(byte);
    }
    std::cout << std::endl;
}
