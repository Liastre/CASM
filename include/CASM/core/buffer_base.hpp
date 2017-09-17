// =============== DESCRIPTION ===============
// Created on 05 Jun.
// ===========================================

#ifndef CASM_BUFFER_BASE_HPP
#define CASM_BUFFER_BASE_HPP

#include <chrono>
#include <vector>
#include <fstream>

union bitsRep{
    int32_t int32;
    int16_t int16[2];
    int8_t int8[4];
};

class BufferStorage {
public:
    BufferStorage();
    explicit BufferStorage(uint32_t size);
    ~BufferStorage() = default;

    void read(std::fstream& stream);
    void read(void* arrayPtr, uint32_t sizeInBytes);
    void write(std::fstream& stream);
    void write(BufferStorage* data);
    void write(void* arrayPtr, uint32_t sizeInBytes);
    void copy(BufferStorage* data);
    void clear();
    uint32_t getSize();

protected:
    std::vector<uint8_t> buffer;
    uint32_t filled;
    uint32_t size;
};

#endif //CASM_BUFFER_BASE_HPP
