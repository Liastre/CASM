// =============== DESCRIPTION ===============
// class for Buffer
// on copying, new object uses the same data storage
// ===========================================

#ifndef CASM_BUFFER_HPP
#define CASM_BUFFER_HPP

#include "buffer_base.hpp"
#include "wave_properties.hpp"
#include <memory>


/// @class Buffer
/// @brief storage audio data. On copying points to the same data array in memory
class Buffer {
public:
    Buffer();
    Buffer(WaveProperties waveProperties, uint32_t framesCount);
    Buffer(WaveProperties waveProperties, std::chrono::duration<double> duration);
    Buffer(uint32_t samplesPerSecond, uint32_t framesCount, uint16_t blockAlign);
    ~Buffer();

    void read(std::ofstream &stream);

    /// @brief writes data from input buffer to current
    /// @param [in] data - copying from
    void write(Buffer data);

    /// @brief writes raw data into the buffer
    /// @param [in] arrayPtr - pointer to begin of array
    /// @param [in] arraySize - actual array size
    /// @param [in] sizeOfTypeInBytes
    void write(void* arrayPtr, const uint32_t arraySize, const uint8_t sizeOfTypeInBytes);

    /// @brief template method that writes raw data into the buffer
    /// @tparam PassedType - type of passed array
    /// @param [in] arrayPtr - pointer to begin of array
    /// @param [in] arraySize - actual array size
    template <typename PassedType>
    void write(void* arrayPtr, const uint32_t arraySize) {
        // static checks
        static_assert(std::is_integral<PassedType>::value, "Only integer types are allowed");

        write(arrayPtr, arraySize, sizeof(PassedType));
    }

    void copy(Buffer data);

    uint32_t getSize();
    std::chrono::duration<double> getDuration();

private:
    void init(uint32_t samplesPerSecond, uint32_t framesCount, uint16_t blockAlign);

    std::shared_ptr<BufferBase> buffer;
    std::chrono::duration<double> duration; ///< approximate duration in time
    uint32_t framesCount; ///< frames needed for requested duration
    uint16_t blockAlign;
};

#endif //CASM_BUFFER_HPP
