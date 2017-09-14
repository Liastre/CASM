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

class BufferInterface {
public:
    virtual void read(std::fstream& stream)=0;
    virtual void write(BufferInterface* data)=0;
    virtual void write(void* arrayPtr, uint32_t sizeInBytes)=0;
    virtual void copy(BufferInterface* data)=0;
};

class BufferBase {
public:
    BufferBase();
    BufferBase(uint32_t size);
    ~BufferBase() = default;

    void read(std::fstream& stream);
    void read(void* arrayPtr, uint32_t sizeInBytes);
    void write(std::fstream& stream);
    void write(BufferBase* data);
    void write(void* arrayPtr, uint32_t sizeInBytes);
    void copy(BufferBase* data);
    void clear();
    uint32_t getSize();

protected:
    std::vector<uint8_t> buffer;
    uint32_t filled;
    uint32_t size;
};

#endif //CASM_BUFFER_BASE_HPP
