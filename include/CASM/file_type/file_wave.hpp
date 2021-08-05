#ifndef CASM_FILE_WAVE_HPP
#define CASM_FILE_WAVE_HPP

#include <CASM/core/file_base.hpp>


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

class FileWave : public FileBase {
public:
    FileWave() = default;
    explicit FileWave(std::string &filePath);
    ~FileWave() final;

    // FileBase interface
    BufferStatus read(Buffer & buffer) final;
    bool write(Buffer const & buffer) final;
    WaveProperties readHeader() final;
    bool writeHeader(WaveProperties const& waveProperties) final;
    bool finalize() final;

private:
    WavHeader wavHeader;
    int64_t posDataChunk = 0;
    int64_t posFileLength = 0;
    bool finalized = false;
};

}

#endif //CASM_FILE_WAVE_HPP
