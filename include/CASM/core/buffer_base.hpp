/**
    @file buffer_base.hpp
    @copyright LGPLv3
    @brief declaration of BufferStorage class
**/

#pragma once

#include <CASM/CASM.hpp>
#include <CASM/core/circle_buffer.hpp>
#include <atomic>
#include <chrono>
#include <vector>
#include <fstream>

// TODO: rename file to buffer_storage
namespace CASM {

enum class BufferStatus {
    BufferFilled,
    BufferEmpty,
    DataEmpty,
    DataFilled,
};

/**
 * Storage for sequence of bytes
 */
class BufferStorage {
public:
    BufferStorage();
    explicit BufferStorage(std::size_t size);
    ~BufferStorage() = default;

    /**
     * Read buffer to fstream. Copies all the data
     * from buffer to fstream.
     * @param[in,out] stream
     */
    void read(std::fstream& stream) const;
    void read(void* arrayPtr, std::size_t sizeInBytes) const;
    BufferStatus write(std::fstream& stream);
    void write(BufferStorage const& data);
    void write(void* arrayPtr, std::size_t sizeInBytes);
    void copy(BufferStorage const& data);
    void clear();
    std::size_t getSize();

protected:
    using Byte = std::uint8_t;
    using ByteBuffer = CircleBuffer<Byte>;

    ByteBuffer _buffer;
};

} // namespace CASM
