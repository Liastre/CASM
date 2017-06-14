// =============== DESCRIPTION ===============
// Created on 05 Jun.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_BUFFER_BASE_HPP
#define CROSSAUDIOSTREAMMANAGER_BUFFER_BASE_HPP

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
    BufferBase(){};
    virtual ~BufferBase() {};
    virtual void* read()=0;
    virtual void read(std::ofstream &stream)=0;
    virtual bool writeSingle(void* ptr)=0;
    virtual bool writeArray(void* ptr, uint32_t paramSize)=0;
    virtual bool writeVector(void* ptr, uint32_t paramSize)=0;
    virtual uint32_t getSize()=0;
    virtual bool writeArrayTemplate(void* arrayPtr, uint32_t arraySize, uint8_t typeSize)=0;
};

template<typename T>
class BufferTemplate : public BufferBase {
    static_assert(
            std::is_same<int8_t, T>::value ||
            std::is_same<uint8_t, T>::value,
            "Type doesn't support");
public:
    BufferTemplate();
    BufferTemplate(uint32_t paramSize);
    ~BufferTemplate();
    void* read() override {
        return &buffer;
    }

    //TODO: to file write
    void read(std::ofstream &stream) {
        /*for(uint32_t i = 0; i<filled; i++) {
            stream.write((const char*) &buffer[i], sizeof(T));
        }*/
        bitsRep val;
        for(uint32_t i = 0; i<filled; i+=2) {
            val.int8[0] = buffer[i];
            val.int8[1] = buffer[i+1];
            int16_t tmp = val.int16[0];
            stream.write((const char*) &tmp, 2);
        }
        buffer.clear();
        filled = 0;
    };

    bool writeSingle(void* ptr) override;
    bool writeVector(void* ptr, uint32_t paramSize) override;
    bool writeArray(void* ptr, uint32_t paramSize) override;
    bool writeArrayTemplate(void* arrayPtr, uint32_t arraySize, uint8_t typeSize)override;

    uint32_t getSize() {
        return size;
    }

private:
    uint32_t filled;
    uint32_t size;
    std::vector<T> buffer;
};

#include "buffer_base.inl"

#endif //CROSSAUDIOSTREAMMANAGER_BUFFER_BASE_HPP
