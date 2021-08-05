#pragma once

#include <CASM/file.hpp>


namespace CASM {

typedef struct {
    std::array< char, 4 > chunkID       {'R','I','F','F'};  // chunk
    uint32_t chunkSize                  {0};
    std::array< char, 4 > chunkFormat   {'W','A','V','E'};
    std::array< char, 4 > fmtID         {'f','m','t',' '};  // sub-chunk 1
    uint32_t fmtSize                    {0};
    uint16_t fmtAudioFormat             {0};
    uint16_t fmtNumChannels             {0};
    uint32_t fmtSampleRate              {0};
    uint32_t fmtByteRate                {0};
    uint16_t fmtBlockAlign              {0};
    uint16_t fmtBitsPerSample           {0};
    uint16_t fmtExtraParamSize          {0};
    char *fmtExtraParams                {nullptr};
    std::array< char, 4 > dataID        {'d','a','t','a'};  // sub-chunk 2
    uint32_t dataSize                   {0};
} WavHeader;

class FileWave : public FileInterface {
public:
    FileWave() = default;
    explicit FileWave(std::string const& filePath);

    // FileInterface
    bool open(Access access) final;
    bool close() final;
    bool isGood() const final;

    WaveProperties readHeader() final;
    bool writeHeader(WaveProperties const& waveProperties) final;
    BufferStatus readData(Buffer& buffer) final;
    bool writeData(Buffer const& buffer) final;
    bool finalize() final;

private:
    WavHeader wavHeader;
    int64_t posDataChunk = 0;
    int64_t posFileLength = 0;
    bool finalized = false;

    // TODO: fix memory leak
    std::fstream* _stream = new std::fstream();
    std::string _path = "";
};

}
