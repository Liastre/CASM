/**
    @file buffer_base.hpp
    @copyright LGPLv3
    @brief declaration of BufferStorage class
**/

#ifndef CASM_BUFFER_BASE_HPP
#define CASM_BUFFER_BASE_HPP

#include <atomic>
#include <chrono>
#include <vector>
#include <fstream>


namespace CASM {

union bitsRep {
    int32_t int32;
    int16_t int16[2];
    int8_t int8[4];
};

/// @class BufferStorage
class BufferStorage {
public:
    BufferStorage();
    explicit BufferStorage(uint32_t size);
    ~BufferStorage() = default;

    void read(std::fstream &stream) const;
    void read(void *arrayPtr, uint32_t sizeInBytes) const;
    bool write(std::fstream &stream);
    void write(BufferStorage *data);
    void write(void *arrayPtr, uint32_t sizeInBytes);
    void copy(BufferStorage *data);
    void clear();
    uint32_t getSize();

protected:
    /// @brief storage for sequence of bytes
    std::vector< uint8_t > buffer;
    /// @brief amount of filled bytes
    std::atomic< uint32_t > filled;
    /// @brief buffer storage size
    std::atomic< uint32_t > size;
};

} // namespace CASM

#endif //CASM_BUFFER_BASE_HPP
