#include <CASM/core/buffer.hpp>
#include <cassert>

namespace CASM {

Buffer::Buffer() {
    _duration = Duration::zero();
    _framesCount = 0;
}

Buffer::~Buffer() {
    _storage.reset();
}

Buffer::Buffer(WaveProperties const& waveProperties, const uint32_t framesCount) {
    init(waveProperties, framesCount);
}

Buffer::Buffer(WaveProperties const& waveProperties, Duration const& duration) {
    // TODO: round to greater
    auto compFramesCount = (uint32_t)(waveProperties.getSamplesPerSecond() * duration.count());
    init(waveProperties, compFramesCount);
}

void
Buffer::init(WaveProperties const& waveProperties, uint32_t framesCount) {
    Buffer::_waveProperties = waveProperties;
    Buffer::_framesCount = framesCount;
    // compute actual buffer duration
    _duration = std::chrono::duration<double>((double)Buffer::_framesCount / (double)waveProperties.getSamplesPerSecond());
    _storage = std::make_shared<BufferStorage>(Buffer::_framesCount * waveProperties.getBlockAlign());
}

std::size_t
Buffer::getSize() const {
    return _storage->getSize();
}

Duration
Buffer::getDuration() const {
    return _duration;
}

WaveProperties
Buffer::getWaveProperties() const {
    return _waveProperties;
}

void
Buffer::read(DataStream::DataStreamInterface& dataStream) const {
    _storage->read(dataStream);
}

void
Buffer::read(void* arrayPtr, uint32_t sizeInBytes) const {
    _storage->read(arrayPtr, sizeInBytes);
}

BufferStatus
Buffer::write(DataStream::DataStreamInterface& dataStream) {
    return _storage->write(dataStream);
}

void
Buffer::write(Buffer data) {
    _storage->write(*(data._storage));
}

void
Buffer::write(void* arrayPtr, const uint32_t sizeInBytes) {
    _storage->write(arrayPtr, sizeInBytes);
}

//TODO: pass by const ref
void
Buffer::copy(Buffer data) {
    _duration = data._duration;
    _framesCount = data._framesCount;
    _waveProperties = data._waveProperties;
    _storage->copy(*(data._storage));
}

void
Buffer::clear() {
    _storage->clear();
}

} // namespace CASM
