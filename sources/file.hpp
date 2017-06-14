// =============== DESCRIPTION ===============
// Created on 30 May.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_FILE_HPP
#define CROSSAUDIOSTREAMMANAGER_FILE_HPP

#include <CASM/CASM.hpp>
#include "buffer.hpp"
#include "wave_properties.hpp"

#include <string>
#include <fstream>


enum FileType{
    WAV
};

class File {
public:
    File(std::string fileName, CASM::Access access, WaveProperties paramWaveProperties = WaveProperties());
    ~File();
    int write(Buffer buffer);
    int write(uint16_t value);
    bool write(std::vector<uint8_t> arr);

    template <typename T>
    bool write(T* array, uint_fast64_t arraySize) {
        for(uint_fast64_t i=0; i<arraySize; i++) {
            write<T>(stream, array[i]);
        }

        return true;
    }

    int finalize();
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
