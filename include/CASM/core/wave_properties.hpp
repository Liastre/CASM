/**
    @file wave_properties.hpp
    @copyright LGPLv3
    @brief class WaveProperties for storing wave properties
**/

#pragma once

#include <cstdint>

namespace CASM {

class WaveProperties {
public:
    WaveProperties() = default;
    WaveProperties(std::uint16_t channelsCount,
      std::uint32_t samplesPerSecond,
      std::uint16_t bitsPerSample,
      std::uint16_t bitsPerSampleContainerSize,
      bool paramIsSigned = true);

    std::uint16_t getChannelsCount() const;
    std::uint16_t getBitsPerSample() const;
    std::uint16_t getBitsPerSampleContainerSize() const;
    std::uint32_t getSamplesPerSecond() const;
    std::uint32_t getBytesPerSecond() const;
    std::uint16_t getBlockAlign() const;
    /**
     * @return true if signed, false if not
     */
    bool getSign() const;

    bool operator==(WaveProperties const& waveProperties) const;
    bool operator!=(WaveProperties const& waveProperties) const;

private:
    /// @brief channels count
    std::uint16_t _channelsCount = 0;
    /// @brief bits per sample
    std::uint8_t _bitsPerSample = 0;
    /// Container size for sample bits
    std::uint8_t _bitsPerSampleContainerSize = 0;
    /// @brief frame size (size of two integer samples, one for each channel, in bytes)
    std::uint16_t _blockAlign = 0;
    /// @brief samples per second (Hz)
    std::uint32_t _samplesPerSecond = 0;
    /// @brief bytes per second
    std::uint32_t _bytesPerSecond = 0;
    /// @brief true if signed, false if not
    bool _isSigned = false;
};

} // namespace CASM
