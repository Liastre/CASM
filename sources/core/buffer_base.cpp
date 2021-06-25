/**
    @file buffer_base.cpp
    @copyright LGPLv3
    @brief definition for BufferStorage class
**/

#include <CASM/core/buffer_base.hpp>
#include <string>
#include <exception>

namespace CASM {

BufferStorage::BufferStorage() {
    _buffer = ByteBuffer();
}


BufferStorage::BufferStorage(std::size_t size) :BufferStorage() {
    _buffer = ByteBuffer(size);
}


// TODO: const fstream?
void BufferStorage::read(std::fstream & stream) const {
    Byte byte;

    while (_buffer.pop(byte)) {
        stream.write(reinterpret_cast<const char *>(&byte), 1);
    }
}


void BufferStorage::read(void *arrayPtr, const std::size_t sizeInBytes) const {
    auto array = static_cast<uint8_t *>(arrayPtr);
    if (!_buffer.pop(array, sizeInBytes)) {
        throw std::runtime_error("Buffer doesn't have " + std::to_string(sizeInBytes) + " bytes");
    }
}


BufferStatus BufferStorage::write(std::fstream & stream) {
    Byte byte;
    while (true) {
        if (stream.eof())
            return BufferStatus::DataEmpty;
        if (_buffer.full())
            return BufferStatus::BufferFilled;

        stream.read(reinterpret_cast<char *>(&byte), 1);
        _buffer.push(byte);
    }
}


void BufferStorage::write(BufferStorage const & data) {
    _buffer.push(data._buffer);
}


void BufferStorage::write(void *arrayPtr, std::size_t const sizeInBytes) {
    auto array = static_cast<uint8_t *>(arrayPtr);
    if (!_buffer.push(array, sizeInBytes)) {
        throw std::runtime_error("Buffer doesn't have " + std::to_string(sizeInBytes) + " bytes");
    }
}


void BufferStorage::copy(BufferStorage const & data) {
    _buffer.clear();
    _buffer.push(data._buffer);
}


void BufferStorage::clear() {
    _buffer.clear();
}


std::size_t BufferStorage::getSize() {
    return _buffer.size();
}

} // namespace CASM
