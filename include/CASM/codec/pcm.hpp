#pragma once

#include <CASM/codec/codec.hpp>
#include <array>


namespace CASM {

namespace Codec {

using WavHeader = struct {
    std::array<char, 4> chunkID     {'R','I','F','F'};  // RIFF chunk
    std::uint32_t chunkSize         {0};                // chunk size in bytes
    std::array<char, 4> chunkFormat {'W','A','V','E'};  // file type
    std::array<char, 4> fmtID       {'f','m','t',' '};  // FMT sub-chunk
    std::uint32_t fmtSize           {0};                // size of fmt chunk 16 + extra format bytes
    std::uint16_t fmtAudioFormat    {0};                // format (compression code)
    std::uint16_t fmtNumChannels    {0};
    std::uint32_t fmtSampleRate     {0};
    std::uint32_t fmtByteRate       {0};
    std::uint16_t fmtBlockAlign     {0};
    std::uint16_t fmtBitsPerSample  {0};
    std::uint16_t fmtExtraParamSize {0};
    char *fmtExtraParams            {nullptr};
    std::array<char, 4> dataID      {'d','a','t','a'};  // DATA sub-chunk
    std::uint32_t dataSize          {0};
};

class Pcm : public CodecInterface {
public:
    WaveProperties readHeader(DataStream& fileHandler) final;
    bool writeHeader(DataStream& fileHandler, WaveProperties const& waveProperties) final;
    BufferStatus readData(DataStream& fileHandler, Buffer& buffer) final;
    bool writeData(DataStream& dataStream, Buffer const& buffer) final;
    bool finalize(DataStream& fileHandler) final;

private:
    WavHeader _wavHeader;
    std::size_t _posDataChunk = 0;
    std::size_t _posFileLength = 0;
    bool _isFinalized = false;
};

} // namespace Codec

} // namespace CASM
