// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#include <cassert>
#include "buffer.hpp"

Buffer::Buffer() {
}

Buffer::~Buffer() {
    buffer.reset();
}

Buffer::Buffer(WaveProperties paramWaveProperties, std::chrono::duration<double> paramDuration) {
    duration = std::chrono::duration_cast<std::chrono::seconds>(paramDuration);
    waveProperties = paramWaveProperties;
    blockAlign = (uint8_t)waveProperties.getBlockAlign();
    framesCount = (uint32_t)(paramWaveProperties.getSamplesPerSecond()*duration.count());
    BitsType bitsType = paramWaveProperties.getBitsType();
    switch(bitsType) {
    case PCM_16BIT_SIGNED:
    case PCM_32BIT_SIGNED:
        buffer = std::make_shared< BufferTemplate<int8_t> >(framesCount*blockAlign);
        break;
    case PCM_16BIT_UNSIGNED:
    case PCM_32BIT_UNSIGNED:
        buffer = std::make_shared< BufferTemplate<uint8_t> >(framesCount*blockAlign);
        break;
    default:
        assert(false);
        break;
    }
}

void Buffer::read(std::vector<int16_t> &paramVec) {
    paramVec = *static_cast<std::vector<int16_t>*>(buffer->read());
}

void Buffer::read(std::ofstream &stream) {
    buffer->read(stream);
}

bool Buffer::write(std::vector<int16_t> paramVec) {
    return(buffer->writeVector(&paramVec, (uint32_t)paramVec.size()));
}

bool Buffer::write(std::vector<uint16_t> paramVec) {
    return(buffer->writeVector(&paramVec, (uint32_t)paramVec.size()));
}

std::chrono::duration<double> Buffer::getDuration() {
    return duration;
}

WaveProperties Buffer::getWaveProperties() {
    return waveProperties;
}