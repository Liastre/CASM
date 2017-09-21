/// @file CASM.hpp
/// @brief main header of library

#ifndef CASM_CASM_HPP
#define CASM_CASM_HPP

#include "CASMconfig.hpp"
#include <cstdint>
#include <array>


namespace CASM {

// TODO: handle exclusive types
enum Access {
    READ, READ_EXCLUSIVE, WRITE, WRITE_EXCLUSIVE
};

enum DeviceType {
    CAPTURE, RENDER
};

// TODO: replace
typedef struct {
    std::array< char, 4 > chunkID;
    uint32_t chunkSize;
    std::array< char, 4 > chunkFormat;
    std::array< char, 4 > fmtID;       // sub-chunk 1
    uint32_t fmtSize;
    uint16_t fmtAudioFormat;
    uint16_t fmtNumChannels;
    uint32_t fmtSampleRate;
    uint32_t fmtByteRate;
    uint16_t fmtBlockAlign;
    uint16_t fmtBitsPerSample;
    uint16_t fmtExtraParamSize;
    char *fmtExtraParams;
    std::array< char, 4 > dataID;      // sub-chunk 2
    uint32_t dataSize;
} WavHeader;

} // end of namespace CASM

#endif //CASM_CASM_HPP
