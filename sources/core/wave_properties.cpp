/// @file wave_properties.hpp
/// @brief definition of WaveProperties class

#include <CASM/core/wave_properties.hpp>


namespace CASM {

WaveProperties::WaveProperties() {
    channelsCount = 0;
    samplesPerSecond = 0;
    bitsPerSample = 0;
}


WaveProperties::WaveProperties(std::uint16_t channelsCount, std::uint32_t samplesPerSecond, uint32_t bitsPerSample, bool paramIsSigned) {
    // TODO: remake
    switch (bitsPerSample) {
    case 16:
        isSigned = paramIsSigned;
        WaveProperties::bitsPerSample = 16;
        if (isSigned) {
            bitsType = PCM_16BIT_SIGNED;
        }
        else {
            bitsType = PCM_16BIT_UNSIGNED;
        }
        break;

    case 32:
        isSigned = paramIsSigned;
        WaveProperties::bitsPerSample = 32;
        if (isSigned) {
            bitsType = PCM_32BIT_SIGNED;
        }
        else {
            bitsType = PCM_32BIT_UNSIGNED;
        }
        break;
    }
    WaveProperties::channelsCount = channelsCount;
    WaveProperties::samplesPerSecond = samplesPerSecond;
    blockAlign = (uint16_t) (channelsCount*bitsPerSample/8);
    bytesPerSecond = samplesPerSecond*blockAlign;
}


WaveProperties::WaveProperties(std::uint16_t channelsCount, std::uint32_t samplesPerSecond, BitsType bitsType) {
    switch (bitsType) {
    case PCM_16BIT_SIGNED:
        bitsPerSample = 16;
        isSigned = true;
        break;
    case PCM_16BIT_UNSIGNED:
        bitsPerSample = 16;
        isSigned = false;
        break;
    case PCM_32BIT_SIGNED:
        bitsPerSample = 32;
        isSigned = true;
        break;
    case PCM_32BIT_UNSIGNED:
        bitsPerSample = 32;
        isSigned = false;
        break;
        //default:
    }
    WaveProperties::bitsType = bitsType;
    WaveProperties::channelsCount = channelsCount;
    WaveProperties::samplesPerSecond = samplesPerSecond;
    blockAlign = (uint16_t) (channelsCount*bitsPerSample/8);
    bytesPerSecond = samplesPerSecond*blockAlign;
}


WaveProperties::~WaveProperties() {
}


uint16_t WaveProperties::getChannelsCount() const {
    return channelsCount;
}


uint16_t WaveProperties::getBitsPerSample() const {
    return bitsPerSample;
}


uint32_t WaveProperties::getSamplesPerSecond() const {
    return samplesPerSecond;
}


uint32_t WaveProperties::getBytesPerSecond() const {
    return bytesPerSecond;
}


uint16_t WaveProperties::getBlockAlign() const {
    return blockAlign;
}


bool WaveProperties::getSigned() const {
    return isSigned;
}


BitsType WaveProperties::getBitsType() const {
    return bitsType;
}


bool WaveProperties::operator==(WaveProperties waveProperties) const {
    if (this->channelsCount!=waveProperties.channelsCount)
        return false;
    if (this->samplesPerSecond!=waveProperties.samplesPerSecond)
        return false;
    if (this->bitsPerSample!=waveProperties.bitsPerSample)
        return false;

    return true;
}


bool WaveProperties::operator!=(WaveProperties waveProperties) const {
    return !(*this == waveProperties);
}

}