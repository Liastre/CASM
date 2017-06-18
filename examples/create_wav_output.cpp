#include "CASM/file.hpp"

#include <cmath>

constexpr double two_pi = 6.283185307179586476925286766559;
constexpr double max_amplitude = 32760;  // "volume"


int main()
{
    // Write the audio samples
    // (a single C4 note with a sine wave, fading from left to right)
    unsigned int hz = 44100;
    double frequency = 261.626;  // middle C
    double dur = 2.5;
    std::chrono::milliseconds duration (2500);      // time

    WaveProperties waveProperties(2, hz, PCM_16BIT_SIGNED);
    Buffer buffer(waveProperties, std::chrono::milliseconds(2500));
    File file("sine.wav", CASM::WRITE, waveProperties);

    uint64_t N = (uint64_t)(hz * dur);  // total number of samples
    for (int n = 0; n<N; n++) {
        double amplitude = (double) n/N*max_amplitude;
        double value = std::sin((two_pi*n*frequency)/hz);
        int16_t leftChannelSample = (uint16_t)(amplitude*value); // 16 bit
        int16_t rightChannelSample = (uint16_t)((max_amplitude-amplitude)*value); // 16 bit
        /*if(!buffer.write(std::vector<int16_t>{leftChannelSample, rightChannelSample})) {
            file.write(buffer);
            buffer.write(std::vector<int16_t>{leftChannelSample, rightChannelSample});
        }*/
        int16_t data[2] = {leftChannelSample, rightChannelSample};
        buffer.write<int16_t>(data, 2);
    }

    file.write(buffer);
    file.finalize();

    return 0;
}
