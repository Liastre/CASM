// =============== DESCRIPTION ===============
// Created on 05 Jun.
// ===========================================

#ifndef CASM_BUFFER_BASE_INL
#define CASM_BUFFER_BASE_INL

#include <CASM/core/buffer_base.hpp>

BufferBase::BufferBase(){
    size = 0;
    filled = 0;
};

BufferBase::BufferBase(uint32_t size) : BufferBase() {
    BufferBase::size = size;
    buffer.reserve(size);
}

void BufferBase::read(std::fstream& stream) {
    bitsRep val;
    for(uint32_t i = 0; i<filled; i+=2) {
        val.int8[0] = buffer[i];
        val.int8[1] = buffer[i+1];
        int16_t tmp = val.int16[0];
        stream.write((const char*) &tmp, 2);
    }
    /*for(uint32_t i = 0; i<filled; i++) {
        stream.write((const char*) &(*buffer)[i], 1);
    }*/
    clear();
}

void BufferBase::write(BufferBase* data) {
    uint32_t dataSize = data->getSize();
    if (dataSize > (size-filled)) {
        return;
    }
    for (int i=0; i<dataSize; i++) {
        buffer.emplace_back(data->buffer[i]);
    }
    filled += data->filled;
}

void BufferBase::write(void* arrayPtr, const uint32_t sizeInBytes) {
    if (filled+sizeInBytes>size) {
        // buffer overflow
        return;
    }

    if(arrayPtr==nullptr) {
        for(uint32_t i = 0; i<sizeInBytes; i++) {
            buffer.emplace_back(0);
        }
    } else {
        uint8_t* array = static_cast<uint8_t*>(arrayPtr);
        for(uint32_t i=0; i<sizeInBytes; i++) {
            buffer.emplace_back(array[i]);
        }
    }

    filled += sizeInBytes;
}

void BufferBase::copy(BufferBase* data) {
    if (data->getSize() != size) {
        buffer.reserve(data->getSize());
        size = data->getSize();
    }
    clear();
    for (int i=0; i<size; i++) {
        buffer.emplace_back(data->buffer[i]);
    }
    filled = data->filled;
}

void BufferBase::clear() {
    buffer.clear();
    filled = 0;
}

uint32_t BufferBase::getSize() {
    return size;
}


#endif //CASM_BUFFER_BASE_INL
