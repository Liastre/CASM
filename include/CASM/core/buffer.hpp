// =============== DESCRIPTION ===============
// class for Buffer
// on copying, new object uses the same data storage
// ===========================================

#ifndef CASM_BUFFER_HPP
#define CASM_BUFFER_HPP

#include "buffer_base.hpp"
#include "wave_properties.hpp"
#include <memory>

class Buffer {
public:
    Buffer();
    Buffer(WaveProperties waveProperties, uint32_t framesCount);
    Buffer(WaveProperties waveProperties, std::chrono::duration<double> duration);
    ~Buffer();

    void read(std::vector<int16_t> &paramVec);
    void read(std::ofstream &stream);

    /// write - method that writes raw data into the buffer
    /// @param [in] arrayPtr - pointer to begin of array
    /// @param [in] arraySize - actual array size
    /// @param [in] sizeOfTypeInBytes
    void write(void* arrayPtr, const uint32_t arraySize, const uint8_t sizeOfTypeInBytes);

    /// write - template method that writes raw data into the buffer
    /// @tparam PassedType - type of passed array
    /// @param [in] arrayPtr - pointer to begin of array
    /// @param [in] arraySize - actual array size
    template <typename PassedType>
    void write(void* arrayPtr, const uint32_t arraySize) {
        // static checks
        static_assert(std::is_integral<PassedType>::value, "Only integer types are allowed");

        write(arrayPtr, (uint32_t)(arraySize*blockAlign), sizeof(PassedType));
    }

    std::chrono::duration<double> getDuration();
    WaveProperties getWaveProperties();

private:
    void init(WaveProperties waveProperties, uint32_t framesCount);

    std::shared_ptr<BufferBase> buffer;
    std::chrono::duration<double> duration;
    WaveProperties waveProperties;
    uint32_t framesCount;
    uint8_t blockAlign;
};

#endif //CASM_BUFFER_HPP
