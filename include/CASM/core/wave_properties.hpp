/// @file wave_properties.hpp
/// @brief class WaveProperties for storing wave properties

#ifndef CASM_WAVE_PROPERTIES_HPP
#define CASM_WAVE_PROPERTIES_HPP

#include <cstdint>


enum BitsType {
    PCM_16BIT_SIGNED,
    PCM_16BIT_UNSIGNED,
    PCM_32BIT_SIGNED,
    PCM_32BIT_UNSIGNED
};

namespace CASM {

class WaveProperties {
public:
    WaveProperties();
    // TODO: rewrite using single init method
    WaveProperties(std::uint16_t channelsCount, std::uint32_t samplesPerSecond, BitsType bitsType);
    WaveProperties(std::uint16_t channelsCount, std::uint32_t samplesPerSecond, uint32_t bitsPerSample, bool paramIsSigned = true);
    ~WaveProperties();

    // getters
    uint16_t getChannelsCount() const;
    uint16_t getBitsPerSample() const;
    uint32_t getSamplesPerSecond() const;
    uint32_t getBytesPerSecond() const;
    uint16_t getBlockAlign() const;
    BitsType getBitsType() const;
    bool getSigned() const;

    // operators
    bool operator==(WaveProperties waveProperties) const;
    bool operator!=(WaveProperties waveProperties) const;

private:
    uint16_t channelsCount;     // channels count
    uint16_t bitsPerSample;     // bits per sample
    uint16_t blockAlign;        // frame size (size of two integer samples, one for each channel, in bytes)
    uint32_t samplesPerSecond;  // samples per second (Hz)
    uint32_t bytesPerSecond;    // bytes per second
    bool isSigned;
    BitsType bitsType;
};

}

#endif //CASM_WAVE_PROPERTIES_HPP
