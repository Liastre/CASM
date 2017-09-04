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

void* BufferBase::read() {
    return &buffer[0];
}

void BufferBase::read(std::ofstream& stream) {
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

void BufferBase::writeArray(void* arrayPtr, const uint32_t sizeInBytes) {
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

void BufferBase::clear() {
    buffer.clear();
    filled = 0;
}

uint32_t BufferBase::getSize() {
    return size;
}

#endif //CASM_BUFFER_BASE_INL
