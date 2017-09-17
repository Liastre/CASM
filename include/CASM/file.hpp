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
    File(std::string fileName);
    ~File() override;

    // inherited interface
    Buffer open(std::chrono::duration<double> duration) override;

    bool open(Buffer buffer) final;
    void close() final;
    bool read(Buffer& buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() final;

    // getters
    std::string getName();

private:
    bool readHeader();
    bool writeHeader();
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
