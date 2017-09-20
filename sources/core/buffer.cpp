// =============== DESCRIPTION ===============
// ===========================================

#include <CASM/core/buffer.hpp>
#include <cassert>


namespace CASM {

Buffer::Buffer() {
    duration = std::chrono::duration< double >::zero();
    framesCount = 0;
}


Buffer::~Buffer() {
    storage.reset();
}


Buffer::Buffer(const WaveProperties waveProperties, const uint32_t framesCount) {
    init(waveProperties, framesCount);
}


Buffer::Buffer(const WaveProperties waveProperties, const std::chrono::duration< double > duration) {
    // TODO: round to greater
    auto compFramesCount = (uint32_t) (waveProperties.getSamplesPerSecond()*duration.count());
    init(waveProperties, compFramesCount);
}


void Buffer::init(WaveProperties waveProperties, uint32_t framesCount) {
    Buffer::waveProperties = waveProperties;
    Buffer::framesCount = framesCount;
    // compute actual buffer duration
    duration = std::chrono::duration< double >((double) Buffer::framesCount/(double) waveProperties.getSamplesPerSecond());
    storage = std::make_shared< BufferStorage >(Buffer::framesCount*waveProperties.getBlockAlign());
}


uint32_t Buffer::getSize() const {
    return storage->getSize();
}


std::chrono::duration< double > Buffer::getDuration() const {
    return duration;
}


const WaveProperties &Buffer::getWaveProperties() const {
    return waveProperties;
}


void Buffer::read(std::fstream &stream) {
    storage->read(stream);
}


void Buffer::read(void *arrayPtr, uint32_t sizeInBytes) {
    storage->read(arrayPtr, sizeInBytes);
}


void Buffer::write(std::fstream &stream) {
    storage->write(stream);
}


void Buffer::write(Buffer data) {
    storage->write(data.storage.get());
}


void Buffer::write(void *arrayPtr, const uint32_t arraySize, const uint8_t sizeOfTypeInBytes) {
    storage->write(arrayPtr, (uint32_t) (arraySize*sizeOfTypeInBytes));
}


void Buffer::copy(Buffer data) {
    duration = data.duration;
    framesCount = data.framesCount;
    waveProperties = data.waveProperties;
    storage->copy(data.storage.get());
}

}
