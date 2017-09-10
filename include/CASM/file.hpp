// =============== DESCRIPTION ===============
// Created on 30 May.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_FILE_HPP
#define CROSSAUDIOSTREAMMANAGER_FILE_HPP

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
    ~File();

    // interface
    void open(CASM::Access access) final;
    void close() final;
    bool read(Buffer& buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() final;

    int write(uint16_t value);
    bool write(std::vector<uint8_t> arr);

    template <typename T>
    bool write(T* array, uint_fast64_t arraySize) {
        for(uint_fast64_t i=0; i<arraySize; i++) {
            write<T>(stream, array[i]);
        }

        return true;
    }

    std::string getName();

private:
    bool writeFile(WaveProperties waveProperties);
    bool readFile();
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
    std::ofstream stream;
    int64_t posDataChunk;
    int64_t posFileLength;
};

#endif //CROSSAUDIOSTREAMMANAGER_FILE_HPP
