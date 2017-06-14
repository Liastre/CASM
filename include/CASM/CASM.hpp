// =============== DESCRIPTION ===============
// Created on 24 May.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_CASM_HPP
#define CROSSAUDIOSTREAMMANAGER_CASM_HPP

#include "CASMconfig.hpp"

#include <cstdint>


namespace CASM {

//TODO: handle exclusive types
enum Access {
    READ,
    READ_EXCLUSIVE,
    WRITE,
    WRITE_EXCLUSIVE
};

enum DeviceType {
    CAPTURE,
    RENDER
};

typedef struct {
    char        chunkID[4];
    uint32_t    chunkSize;
    char        chunkFormat[4];
    char        fmtID[4];       // sub-chunk 1
    uint32_t    fmtSize;
    uint16_t    fmtAudioFormat;
    uint16_t    fmtNumChannels;
    uint32_t    fmtSampleRate;
    uint32_t    fmtByteRate;
    uint16_t    fmtBlockAlign;
    uint16_t    fmtBitsPerSample;
    uint16_t    fmtExtraParamSize;
    char*       fmtExtraParams;
    char        dataID[4];      // sub-chunk 2
    uint32_t    dataSize;
} WavHeader;

} // end of namespace CASM

#endif //CROSSAUDIOSTREAMMANAGER_CASM_HPP
