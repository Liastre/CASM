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
    init(waveProperties.getSamplesPerSecond(), framesCount, waveProperties.getBlockAlign());
}

Buffer::Buffer(WaveProperties waveProperties, std::chrono::duration<double> duration) {
    // TODO: round to greater
    uint32_t compFramesCount = (uint32_t)(waveProperties.getSamplesPerSecond()*duration.count());
    init(waveProperties.getSamplesPerSecond(), compFramesCount, waveProperties.getBlockAlign());
}

Buffer::Buffer(uint32_t samplesPerSecond, uint32_t framesCount, uint16_t blockAlign) {
    init(samplesPerSecond, framesCount, blockAlign);
}

void Buffer::init(uint32_t samplesPerSecond, uint32_t framesCount, uint16_t blockAlign)
{
    Buffer::blockAlign = blockAlign;
    Buffer::framesCount = framesCount;
    // compute actual buffer duration
    duration = std::chrono::duration<double>((double)Buffer::framesCount/(double)samplesPerSecond);
    buffer = std::make_shared< BufferBase >(Buffer::framesCount*blockAlign);
}

void Buffer::read(std::ofstream &stream) {
    buffer->read(stream);
}

void Buffer::write(Buffer data) {
    buffer->write(data.buffer.get());
}

void Buffer::write(void* arrayPtr, uint32_t arraySize, uint8_t sizeOfTypeInBytes) {
    buffer->write(arrayPtr, (uint32_t) (arraySize*sizeOfTypeInBytes));
}

std::chrono::duration<double> Buffer::getDuration() {
    return duration;
}

uint32_t Buffer::getSize() {
    return buffer->getSize();
}

void Buffer::copy(Buffer data) {
    duration = data.duration;
    framesCount = data.framesCount;
    blockAlign = data.blockAlign;
    buffer->copy(data.buffer.get());
}
