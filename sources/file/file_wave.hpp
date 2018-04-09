#ifndef CASM_FILE_WAVE_HPP
#define CASM_FILE_WAVE_HPP

#include <CASM/core/file_base.hpp>


namespace CASM {

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

class FileWave : public FileBase {
public:
    FileWave() = default;
    explicit FileWave(std::string &filePath);
    ~FileWave() final;

    // FileBase interface
    bool read(Buffer & buffer) final;
    bool write(Buffer const & buffer) final;
    bool readHeader() final;
    bool writeHeader() final;
    bool finalize() final;

private:
    WavHeader wavHeader;
    int64_t posDataChunk = 0;
    int64_t posFileLength = 0;
    bool finalized = false;
};

}

#endif //CASM_FILE_WAVE_HPP
