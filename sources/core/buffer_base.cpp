/**
    @file buffer_base.cpp
    @copyright LGPLv3
    @brief definition for BufferStorage class
**/

#include <CASM/core/buffer_base.hpp>


namespace CASM {

BufferStorage::BufferStorage() {
    size = 0;
    filled = 0;
}


BufferStorage::BufferStorage(uint32_t size) :BufferStorage() {
    BufferStorage::size = size;
    buffer.reserve(size);
}


// TODO: const fstream?
void BufferStorage::read(std::fstream &stream) const {
    for (uint32_t i = 0; i < filled; i++) {
        stream.write((const char *) &buffer[i], 1);
    }
}


void BufferStorage::read(void *arrayPtr, const uint32_t sizeInBytes) const {
    if (filled > sizeInBytes) {
        // buffer overflow
        throw std::runtime_error("Buffer read overflow!");
    }

    auto array = static_cast<uint8_t *>(arrayPtr);
    for (uint32_t i = 0; i < sizeInBytes; i++) {
        array[i] = buffer[i];
    }
}


bool BufferStorage::write(std::fstream &stream) {
    uint32_t i;
    for (i = filled; i < size; i++) {
        if (stream.eof()) {
            return false;
        }
        stream.read(reinterpret_cast<char *>(&buffer[i]), 1);
        filled++;
    }

    return true;
}


void BufferStorage::write(BufferStorage *data) {
    uint32_t dataSize = data->getSize();
    if (dataSize > (size-filled)) {
        return;
    }
    for (int i = 0; i < dataSize; i++) {
        buffer.emplace_back(data->buffer[i]);
    }
    filled += data->filled;
}


void BufferStorage::write(void *arrayPtr, const uint32_t sizeInBytes) {
    if (filled+sizeInBytes > size) {
        // buffer overflow
        return;
    }

    if (arrayPtr==nullptr) {
        for (uint32_t i = 0; i < sizeInBytes; i++) {
            buffer.emplace_back(0);
        }
    } else {
        auto array = static_cast<uint8_t *>(arrayPtr);
        for (uint32_t i = 0; i < sizeInBytes; i++) {
            buffer.emplace_back(array[i]);
        }
    }

    filled += sizeInBytes;
}


void BufferStorage::copy(BufferStorage *data) {
    if (data->getSize()!=size) {
        buffer.reserve(data->getSize());
        size = data->getSize();
    }
    clear();
    for (int i = 0; i < size; i++) {
        buffer.emplace_back(data->buffer[i]);
    }
    filled += data->filled;
}


void BufferStorage::clear() {
    buffer.clear();
    filled = 0;
}


uint32_t BufferStorage::getSize() {
    return size;
}

} // namespace CASM
