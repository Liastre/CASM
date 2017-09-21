// =============== DESCRIPTION ===============
// class for Buffer
// on copying, new object uses the same data storage
// ===========================================

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
    Buffer(WaveProperties waveProperties, uint32_t framesCount);
    Buffer(WaveProperties waveProperties, std::chrono::duration< double > duration);
    ~Buffer();

    // getters
    const WaveProperties &getWaveProperties() const;
    uint32_t getSize() const;
    std::chrono::duration< double > getDuration() const;

    // methods
    void read(std::fstream &stream);
    void read(void *arrayPtr, uint32_t sizeInBytes);
    bool write(std::fstream &stream);

    /// @brief writes data from input buffer to current
    /// @param [in] data - copying from
    void write(Buffer data);

    /// @brief writes raw data into the buffer
    /// @param [in] arrayPtr - pointer to begin of array
    /// @param [in] arraySize - actual array size
    /// @param [in] sizeOfTypeInBytes
    void write(void *arrayPtr, uint32_t arraySize, uint8_t sizeOfTypeInBytes);


    /// @brief template method that writes raw data into the buffer
    /// @tparam PassedType - type of passed array
    /// @param [in] arrayPtr - pointer to begin of array
    /// @param [in] arraySize - actual array size
    template < typename PassedType >
    void write(void *arrayPtr, const uint32_t arraySize) {
        // static checks
        static_assert(std::is_integral< PassedType >::value, "Only integer types are allowed");

        write(arrayPtr, arraySize, sizeof(PassedType));
    }


    /// @brief copy data to the new storage allocated in memory
    /// @param [in] data - data where we copying
    void copy(Buffer data);

private:
    void init(WaveProperties waveProperties, uint32_t framesCount);

    std::shared_ptr< BufferStorage > storage;
    WaveProperties waveProperties;
    /// @brief approximate duration in time units
    std::chrono::duration< double > duration;
    /// @brief frames needed for requested duration
    uint32_t framesCount;
};

}

#endif //CASM_BUFFER_HPP
