/**
    @file wave_properties.hpp
    @copyright LGPLv3
    @brief class WaveProperties for storing wave properties
**/

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
    /// @return channels count
    uint16_t getChannelsCount() const;
    /// @return bits per sample
    uint16_t getBitsPerSample() const;
    /// @return samples per second
    uint32_t getSamplesPerSecond() const;
    /// @return bytes per second
    uint32_t getBytesPerSecond() const;
    /// @return get block align
    uint16_t getBlockAlign() const;
    /// @return get enum value of bit's type
    BitsType getBitsType() const;
    /// @return true if signed, false if not
    bool getSigned() const;

    // operators
    bool operator==(WaveProperties waveProperties) const;
    bool operator!=(WaveProperties waveProperties) const;

private:
    /// @brief channels count
    uint16_t channelsCount;
    /// @brief bits per sample
    uint16_t bitsPerSample;
    /// @brief frame size (size of two integer samples, one for each channel, in bytes)
    uint16_t blockAlign;
    /// @brief samples per second (Hz)
    uint32_t samplesPerSecond;
    /// @brief bytes per second
    uint32_t bytesPerSecond;
    /// @brief true if signed, false if not
    bool isSigned;
    /// @brief enum bit's type
    BitsType bitsType;
};

} // namespace CASM

#endif //CASM_WAVE_PROPERTIES_HPP
