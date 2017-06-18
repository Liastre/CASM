// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_BUFFER_HPP
#define CROSSAUDIOSTREAMMANAGER_BUFFER_HPP

#include "buffer_base.hpp"
#include "wave_properties.hpp"
#include <memory>

class Buffer {
public:
    Buffer();
    Buffer(WaveProperties waveProperties, std::chrono::duration<double> duration);
    ~Buffer();
    void read(std::vector<int16_t> &paramVec);
    void read(std::ofstream &stream);

    template <typename PassedType>
    bool write(void* arrayPtr, uint32_t framesCount) {
        static_assert(std::is_integral<PassedType>::value,
                "Only integer types are allowed");
        return(buffer->writeArrayTemplate(arrayPtr, framesCount*blockAlign, sizeof(PassedType)));
    }
    bool write(std::vector<int16_t> paramVec);
    bool write(std::vector<uint16_t> paramVec);
    std::chrono::duration<double> getDuration();
    WaveProperties getWaveProperties();

private:
    std::shared_ptr<BufferBase> buffer;
    std::chrono::duration<double> duration;
    WaveProperties waveProperties;
    uint32_t framesCount;
    uint8_t blockAlign;
};

#endif //CROSSAUDIOSTREAMMANAGER_BUFFER_HPP
