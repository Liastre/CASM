// =============== DESCRIPTION ===============
// Created on 05 Jun.
// ===========================================

#ifndef CASM_BUFFER_BASE_HPP
#define CASM_BUFFER_BASE_HPP

#include "wave_properties.hpp"
#include <chrono>
#include <vector>
#include <fstream>

union bitsRep{
    int32_t int32;
    int16_t int16[2];
    int8_t int8[4];
};

class BufferBase {
public:
    BufferBase();
    BufferBase(uint32_t size);
    ~BufferBase() = default;

    void* read();
    void read(std::ofstream& stream);
    void writeArray(void* arrayPtr, const uint32_t sizeInBytes);
    void clear();
    uint32_t getSize();

protected:
    std::vector<uint8_t> buffer;
    uint32_t filled;
    uint32_t size;
};

#endif //CASM_BUFFER_BASE_HPP
