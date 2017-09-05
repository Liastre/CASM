// =============== DESCRIPTION ===============
// ===========================================

#include <CASM/core/buffer.hpp>
#include <cassert>

Buffer::Buffer() {
}

Buffer::~Buffer() {
    buffer.reset();
}

Buffer::Buffer(WaveProperties waveProperties, uint32_t framesCount) {
    init(waveProperties, framesCount);
}

Buffer::Buffer(WaveProperties waveProperties, std::chrono::duration<double> duration) {
    uint32_t compFramesCount = (uint32_t)(waveProperties.getSamplesPerSecond()*duration.count());
    init(waveProperties, compFramesCount);
}

void Buffer::init(WaveProperties waveProperties, uint32_t framesCount)
{
    Buffer::waveProperties = waveProperties;
    Buffer::framesCount = framesCount;
    // compute actual buffer duration
    duration = std::chrono::duration<double>((double)Buffer::framesCount/(double)waveProperties.getSamplesPerSecond());
    blockAlign = (uint8_t)waveProperties.getBlockAlign();
    BitsType bitsType = waveProperties.getBitsType();
    buffer = std::make_shared< BufferBase >(Buffer::framesCount*blockAlign);
}

void Buffer::write(void* arrayPtr, uint32_t arraySize, uint8_t sizeOfTypeInBytes) {
    buffer->writeArray(arrayPtr, (uint32_t) (arraySize*sizeOfTypeInBytes));
}

void Buffer::read(std::vector<int16_t> &paramVec) {
    paramVec = *static_cast<std::vector<int16_t>*>(buffer->read());
}

void Buffer::read(std::ofstream &stream) {
    buffer->read(stream);
}

std::chrono::duration<double> Buffer::getDuration() {
    return duration;
}

WaveProperties Buffer::getWaveProperties() {
    return waveProperties;
}
