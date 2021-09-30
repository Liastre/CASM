#include <CASM/core/wave_properties.hpp>
#include <stdexcept>
#include <cassert>

namespace CASM {

WaveProperties::WaveProperties(
  std::uint16_t channelsCount,
  std::uint32_t samplesPerSecond,
  std::uint16_t bitsPerSample,
  std::uint16_t bitsPerSampleContainerSize,
  bool isSigned) {
    _bitsPerSample = bitsPerSample;
    _bitsPerSampleContainerSize = bitsPerSampleContainerSize;
    _isSigned = isSigned;
    _channelsCount = channelsCount;
    _samplesPerSecond = samplesPerSecond;
    _blockAlign = static_cast<std::uint16_t>(_channelsCount * _bitsPerSampleContainerSize / 8);
    _bytesPerSecond = samplesPerSecond * _blockAlign;
}

std::uint16_t
WaveProperties::getChannelsCount() const {
    return _channelsCount;
}

std::uint16_t
WaveProperties::getBitsPerSample() const {
    return _bitsPerSample;
}

std::uint16_t
WaveProperties::getBitsPerSampleContainerSize() const {
    return _bitsPerSampleContainerSize;
}

std::uint32_t
WaveProperties::getSamplesPerSecond() const {
    return _samplesPerSecond;
}

std::uint32_t
WaveProperties::getBytesPerSecond() const {
    return _bytesPerSecond;
}

std::uint16_t
WaveProperties::getBlockAlign() const {
    return _blockAlign;
}

bool
WaveProperties::getSign() const {
    return _isSigned;
}

bool
WaveProperties::operator==(WaveProperties const& waveProperties) const {
    if (_channelsCount != waveProperties._channelsCount)
        return false;
    if (_samplesPerSecond != waveProperties._samplesPerSecond)
        return false;
    if (_bitsPerSample != waveProperties._bitsPerSample)
        return false;

    return true;
}

bool
WaveProperties::operator!=(WaveProperties const& waveProperties) const {
    return !(*this == waveProperties);
}

} // namespace CASM
