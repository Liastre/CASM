// =============== DESCRIPTION ===============
// Created on 30-Sep-17.
// ===========================================

#ifndef CASM_FILE_WAVE_HPP
#define CASM_FILE_WAVE_HPP

#include <CASM/core/file_base.hpp>


namespace CASM {

class FileWave : public FileBase {
public:
    FileWave() = default;
    FileWave(std::string fileName);
    ~FileWave() final;

    // FileBase interface
    bool read(Buffer &buffer) final;
    bool write(Buffer buffer) final;
    bool readHeader() final;
    bool writeHeader() final;
    bool finalize() final;

private:
    CASM::WavHeader wavHeader;
    int64_t posDataChunk = 0;
    int64_t posFileLength = 0;
    bool finalized = false;
};

}

#endif //CASM_FILE_WAVE_HPP
