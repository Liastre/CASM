// =============== DESCRIPTION ===============
// Created on 30 May.
// ===========================================

#ifndef CASM_FILE_HPP
#define CASM_FILE_HPP

#include <CASM/CASM.hpp>
#include <CASM/core/end_point.hpp>
#include "CASM/core/buffer.hpp"
#include "CASM/core/wave_properties.hpp"

#include <string>
#include <fstream>

enum FileType{
    WAV
};

class File final : public EndPointBase {
public:
    File(std::string fileName, WaveProperties paramWaveProperties = WaveProperties());
    ~File() override;

    // interface
    void open(CASM::Access access) final;
    void close() final;
    bool read(Buffer& buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() final;

    std::string getName();

private:
    bool writeHeader();
    bool readHeader();
    bool isExist(const std::string& name);
    bool splitExtension(const std::string& fileName);
    bool generateName();

    template<typename T>
    void write(std::ofstream& stream, const T& t)
    {
        stream.write((const char*) &t, sizeof(T));
    }

private:
    bool finalized;
    CASM::WavHeader wavHeader;
    std::string name;
    std::string path;
    std::string extension;
    std::fstream stream;
    int64_t posDataChunk;
    int64_t posFileLength;
};

#endif //CASM_FILE_HPP
