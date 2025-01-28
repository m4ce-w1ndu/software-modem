#include "ModemAudioPlayer.h"

namespace swmodem {

ModemAudioPlayer::ModemAudioPlayer() : stream(nullptr), is_initialized(false)
{
    if (const PaError err = Pa_Initialize(); paNoError != err) {
        const std::string err_text(Pa_GetErrorText(err));
        throw std::runtime_error("PortAudio initialization failed: " + err_text);
    }

    is_initialized = true;
}

ModemAudioPlayer::~ModemAudioPlayer()
{
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }

    if (is_initialized) {
        Pa_Terminate();
    }
}

void ModemAudioPlayer::play(const std::vector<double> &waveform, double sample_rate)
{
    AudioData data = {&waveform, 0};

    PaStreamCallback* streamCb;
    if (const PaError err = Pa_OpenDefaultStream(
        &stream,
        0,
        1,
        paFloat32,
        sample_rate,
        256,
        audio_callback,
        &data); paNoError != err) {
        const std::string err_text(Pa_GetErrorText(err));
        throw std::runtime_error("PortAudio stream opening failed: " + err_text);
    }

    if (const PaError err = Pa_StartStream(stream); paNoError != err) {
        const std::string err_text(Pa_GetErrorText(err));
        throw std::runtime_error("PortAudio stream start failed: " + err_text);
    }

    while (Pa_IsStreamActive(stream)) {
        Pa_Sleep(100);
    }
}

int ModemAudioPlayer::audio_callback(const void *input, void *output, unsigned long frame_count, const PaStreamCallbackTimeInfo *time_info, PaStreamCallbackFlags status_flags, void *user_data)
{
    auto* data = static_cast<AudioData*>(user_data);
    auto* out = static_cast<double*>(output);

    for (unsigned long i = 0; i < frame_count; ++i) {
        if (data->index < data->waveform->size()) {
            *out++ = (*data->waveform)[data->index++];
        } else {
            *out++ = 0.0;
        }
    }

    return (data->index < data->waveform->size()) ? paContinue : paComplete;
}


}