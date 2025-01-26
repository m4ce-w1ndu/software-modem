#include <SoftwareModem.h>

#include <cstdint>
#include <iostream>

std::vector<uint8_t> string_to_bytes(const std::string& text);
std::string bytes_to_string(const std::vector<uint8_t>& bytes);

int main(int argc, char* argv[])
{
    // Initialize a modem
    swmodem::SoftwareModem modem(44100.0, 1000.0, 500.0);
    std::string input, output;

    // Sample data
    std::cout << "input string to encode: ";
    std::getline(std::cin, input);

    const std::vector<uint8_t> data = string_to_bytes(input);

    // Modulate data
    const std::vector<double> audio_data = modem.modulate(data);
    std::cout << "Generated " << audio_data.size() << " audio samples.\n";

    // Demodulate audio back into data
    std::vector<uint8_t> decoded_data = modem.demodulate(audio_data);
    std::cout << "Decoded data:\n";

    for (uint8_t byte : decoded_data) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << "\n";
    }

    output = bytes_to_string(decoded_data);
    std::cout << "As text: " << output << "\n";

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

std::string bytes_to_string(const std::vector<uint8_t>& bytes)
{
    std::string text;
    std::transform(bytes.cbegin(), bytes.cend(), text.begin(), [](const uint8_t byte) {
        return static_cast<char>(byte);
    });
    return text;
}