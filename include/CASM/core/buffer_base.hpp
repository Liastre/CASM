/**
    @file buffer_base.hpp
    @copyright LGPLv3
    @brief declaration of BufferStorage class
**/

#ifndef CASM_BUFFER_BASE_HPP
#define CASM_BUFFER_BASE_HPP

#include <CASM/CASM.hpp>
#include <CASM/core/circle_buffer.hpp>
#include <atomic>
#include <chrono>
#include <vector>
#include <fstream>

// TODO: rename file to buffer_storage
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
    explicit BufferStorage(std::size_t size);
    ~BufferStorage() = default;

    void read(std::fstream &stream) const;
    void read(void *arrayPtr, std::size_t sizeInBytes) const;
    bool write(std::fstream & stream);
    void write(BufferStorage const & data);
    void write(void *arrayPtr, std::size_t sizeInBytes);
    void copy(BufferStorage const & data);
    void clear();
    std::size_t getSize();

protected:
    using Byte = std::uint8_t;
    using ByteBuffer = CircleBuffer<Byte>;

    /// @brief storage for sequence of bytes
    ByteBuffer _buffer;
};

} // namespace CASM

#endif //CASM_BUFFER_BASE_HPP
