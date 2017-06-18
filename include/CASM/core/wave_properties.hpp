// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#ifndef CASM_WAVE_PROPERTIES_HPP
#define CASM_WAVE_PROPERTIES_HPP

#include <cstdint>

enum BitsType {
    PCM_16BIT_SIGNED,
    PCM_16BIT_UNSIGNED,
    PCM_32BIT_SIGNED,
    PCM_32BIT_UNSIGNED
};

class WaveProperties {
public:
    WaveProperties();
    WaveProperties(std::uint16_t channelsCount, std::uint32_t samplesPerSecond, BitsType bitsType);
    WaveProperties(std::uint16_t paramChannelsCount, std::uint32_t paramSamplesPerSecond, uint32_t paramBitsPerSample, bool paramIsSigned = true);
    ~WaveProperties();
    uint16_t getChannelsCount();
    uint16_t getBitsPerSample();
    uint32_t getSamplesPerSecond();
    uint32_t getBytesPerSecond();
    uint16_t getBlockAlign();
    BitsType getBitsType();
    bool getSigned();
    bool operator==(WaveProperties waveProperties);

private:
    uint16_t channelsCount;     // channels count
    uint16_t bitsPerSample;     // bits per sample
    uint16_t blockAlign;        // frame size (size of two integer samples, one for each channel, in bytes)
    uint32_t samplesPerSecond;  // samples per second (Hz)
    uint32_t bytesPerSecond;    // bytes per second
    bool isSigned;
    BitsType bitsType;
};

#endif //CASM_WAVE_PROPERTIES_HPP
