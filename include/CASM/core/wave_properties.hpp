/**
 * Class WaveProperties for storing wave properties
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include <cstdint>

namespace CASM {

class WaveProperties {
public:
    WaveProperties() = default;
    explicit WaveProperties(std::uint16_t channelsCount,
      std::uint32_t samplesPerSecond,
      std::uint16_t bitsPerSample,
      std::uint16_t bitsPerSampleContainerSize,
      bool isSigned);

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
    /// Channels count
    std::uint16_t _channelsCount = 0;
    /// Bits per sample
    std::uint8_t _bitsPerSample = 0;
    /// Container size for sample bits
    std::uint8_t _bitsPerSampleContainerSize = 0;
    /// Size of two integer samples, one for each channel, in bytes (frame size)
    std::uint16_t _blockAlign = 0;
    /// Samples per second (Hz)
    std::uint32_t _samplesPerSecond = 0;
    /// Bytes per second
    std::uint32_t _bytesPerSecond = 0;
    /// Is signed type stream. True if signed, false if not
    bool _isSigned = false;
};

} // namespace CASM
