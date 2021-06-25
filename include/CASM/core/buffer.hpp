/// @file buffer.hpp
/// @brief class Buffer for storage transferring data
/**
    Details:
    On copying, new object uses the same data storage, since
    shared_ptr is used for storage.
    To copy, use Buffer.copy(Buffer) method, it copies data
    of passed buffer to this.
    Don't forget to clear buffer before fill with new data
**/

#ifndef CASM_BUFFER_HPP
#define CASM_BUFFER_HPP

#include "buffer_base.hpp"
#include "wave_properties.hpp"
#include <memory>


namespace CASM {

/// @class Buffer
/// @brief storage audio data. On copying points to the same data array in memory
class Buffer {
public:
    Buffer();
    Buffer(WaveProperties const & waveProperties, uint32_t framesCount);
    Buffer(WaveProperties const & waveProperties, Duration const & duration);
    ~Buffer();

    // getters
    WaveProperties getWaveProperties() const;
    std::size_t getSize() const;
    std::chrono::duration< double > getDuration() const;

    // methods
    void read(std::fstream &stream) const;
    void read(void *arrayPtr, uint32_t sizeInBytes) const;
    BufferStatus write(std::fstream &stream);

    /// @brief writes data from input buffer to current
    /// @param [in] data - copying from
    void write(Buffer data);

    /// @brief writes raw data into the buffer
    /// @param [in] arrayPtr - pointer to begin of array
    /// @param [in] arraySize - actual array size
    /// @param [in] sizeOfTypeInBytes
    void write(void *arrayPtr, uint32_t sizeInBytes);

    /// @brief copy data to the new storage allocated in memory
    /// @param [in] data - data where we copying
    void copy(Buffer data);

    /// @brief clear the buffer
    void clear();

private:
    void init(WaveProperties const & waveProperties, uint32_t framesCount);

    std::shared_ptr< BufferStorage > _storage;
    WaveProperties _waveProperties;
    /// @brief approximate duration in time units
    Duration _duration;
    /// @brief frames needed for requested duration
    uint32_t _framesCount;
};

}

#endif //CASM_BUFFER_HPP
