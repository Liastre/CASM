// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#include <CASM/core/wave_properties.hpp>

WaveProperties::WaveProperties() {
    channelsCount = 0;
    samplesPerSecond = 0;
    bitsPerSample = 0;
}

WaveProperties::WaveProperties(std::uint16_t paramChannelsCount, std::uint32_t paramSamplesPerSecond, uint32_t paramBitsPerSample, bool paramIsSigned) {
    switch (paramBitsPerSample) {
    case 16:
        isSigned = paramIsSigned;
        bitsPerSample = 16;
        if(isSigned) {
            bitsType = PCM_16BIT_SIGNED;
        } else {
            bitsType = PCM_16BIT_UNSIGNED;
        }
        break;
    case 32:
        isSigned = paramIsSigned;
        bitsPerSample = 32;
        if(isSigned) {
            bitsType = PCM_32BIT_SIGNED;
        } else {
            bitsType = PCM_32BIT_UNSIGNED;
        }
        break;
    }
    channelsCount = paramChannelsCount;
    samplesPerSecond = paramSamplesPerSecond;
    blockAlign = (uint16_t)(channelsCount * bitsPerSample / 8);
    bytesPerSecond = samplesPerSecond * blockAlign;
}

WaveProperties::WaveProperties(std::uint16_t paramChannelsCount, std::uint32_t paramSamplesPerSecond, BitsType paramBitsType) {
    switch(bitsType) {
    case PCM_16BIT_SIGNED:
        bitsPerSample = 16;
        isSigned = true;
        break;
    case PCM_16BIT_UNSIGNED:
        bitsPerSample = 16;
        isSigned = false;
        break;
    //default:
    }
    bitsType = paramBitsType;
    channelsCount = paramChannelsCount;
    samplesPerSecond = paramSamplesPerSecond;
    blockAlign = (uint16_t)(channelsCount * bitsPerSample / 8);
    bytesPerSecond = samplesPerSecond * blockAlign;
}

WaveProperties::~WaveProperties() {
}

uint16_t WaveProperties::getChannelsCount() {
    return channelsCount;
}

uint16_t WaveProperties::getBitsPerSample() {
    return bitsPerSample;
}

uint32_t WaveProperties::getSamplesPerSecond() {
    return samplesPerSecond;
}

uint32_t WaveProperties::getBytesPerSecond() {
    return bytesPerSecond;
}

uint16_t WaveProperties::getBlockAlign() {
    return blockAlign;
}

bool WaveProperties::getSigned() {
    return isSigned;
}

BitsType WaveProperties::getBitsType() {
    return bitsType;
}

bool WaveProperties::operator==(WaveProperties waveProperties) {
    if(this->channelsCount != waveProperties.channelsCount)
        return false;
    if(this->samplesPerSecond != waveProperties.samplesPerSecond)
        return false;
    if(this->bitsPerSample != waveProperties.bitsPerSample)
        return false;

    return true;
}