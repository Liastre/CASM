// =============== DESCRIPTION ===============
// Created on 30 May.
// ===========================================

#include "CASM/file.hpp"

#include <cstdint>
#include <cstring>
#include <cassert>

namespace little_endian {
template<typename T>
void write(std::ofstream& stream, const T& t)
{
    stream.write((const char*) &t, sizeof(T));
}
}

// TODO: remake
namespace big_endian {
template <typename Word>
std::ostream& write( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
{
    for (; size; --size, value >>= 8)
        outs.put( static_cast <char> (value & 0xFF) );
    return outs;
}
}

/*template <typename T>
void writeFormat(std::ofstream& stream) {
    embed::write<short>(stream, 1);
}

template <>
void writeFormat<float>(std::ofstream& stream) {
    embed::write<short>(stream, 3);
}

template <typename SampleType>
void writeWAVData(char const* outFile, SampleType* buf, size_t bufSize, int sampleRate, short channels)
{
    std::ofstream stream(outFile, std::ios::binary|std::ios::app|std::ios::ate);

    stream.write("RIFF", 4);                                        // Start writting RIFF
    embed::write<int>(stream, 0);                                          // (file-size)-8 - FOR NOW IGNORED
    stream.write("WAVE", 4);                                        // File type

    stream.write("fmt ", 4);                                        // Start Writting format chunk "fmt"
    embed::write<int>(stream, 16);                                         // Chunk Data Size 16 + extra format bytes
    writeFormat<SampleType>(stream);                                // Format (compression code)
    embed::write<short>(stream, channels);                                 // Channels
    embed::write<int>(stream, sampleRate);                                 // Sample Rate
    embed::write<int>(stream, sampleRate * channels * sizeof(SampleType)); // Byterate (byte/per sec)
    embed::write<short>(stream, channels * sizeof(SampleType));            // Frame size (block align)
    embed::write<short>(stream, 8 * sizeof(SampleType));                   // Bits per sample

    stream.write("data", 4);                                        // Start writting chunk for extra format bytes
    stream.write((const char*)&bufSize, 0);                         //  - FOR NOW IGNORED
    stream.write((const char*)buf, 0);                              //  - FOR NOW IGNORED
}

template <typename Word>
std::ostream& write_word(std::ostream& outs, Word value, unsigned size = sizeof( Word ))
{
    for (; size; --size, value >>= 8)
        outs.put( static_cast <char> (value & 0xFF) );
    return outs;
}*/

File::File(std::string fileName, CASM::Access access, WaveProperties waveProperties) {
    posDataChunk = 0;
    posFileLength = 0;
    finalized = false;
    splitExtension(fileName);

    switch(access){
    case CASM::WRITE:
        generateName();
        path = name + '.' + extension;
        writeFile(waveProperties);
        break;
    case CASM::READ:
        path = fileName;
        if(isExist(path)) {
            readFile();
        }
        break;
    }
}

File::~File() {
    if (!finalized) {
        finalize();
    }
}

bool File::writeFile(WaveProperties waveProperties) {
    stream.open(path, /*std::ios::out | */std::ios::binary);

    // write header
    stream.write("RIFF", 4);                // RIFF chunk
    little_endian::write<uint32_t>(stream, 0);      // RIFF chunk size in bytes
    stream.write("WAVE", 4);                // file type

    stream.write("fmt ", 4);                // fmt chunk
    little_endian::write<uint32_t>(stream, 16);     // size of fmt chunk 16 + extra format bytes
    little_endian::write<uint16_t>(stream, 3);      // format (compression code)

    little_endian::write<uint16_t>(stream, waveProperties.getChannelsCount());
    little_endian::write<uint32_t>(stream, waveProperties.getSamplesPerSecond());
    little_endian::write<uint32_t>(stream, waveProperties.getBytesPerSecond());
    little_endian::write<uint16_t>(stream, waveProperties.getBlockAlign());
    little_endian::write<uint16_t>(stream, waveProperties.getBitsPerSample());

    // Write the data chunk header
    posDataChunk = stream.tellp();
    stream << "data----";  // (chunk size to be filled in later)

    return true;
}

bool File::readFile() {
    std::ifstream file(path, std::ios::binary);

    file.read(wavHeader.chunkID, 4);
    file.read((char*)&wavHeader.chunkSize, 4);
    file.read(wavHeader.chunkFormat, 4);
    file.read(wavHeader.fmtID, 4);
    file.read((char*)&wavHeader.fmtSize, 4);
    file.read((char*)&wavHeader.fmtAudioFormat, 2);
    file.read((char*)&wavHeader.fmtNumChannels, 2);
    file.read((char*)&wavHeader.fmtSampleRate, 4);
    file.read((char*)&wavHeader.fmtByteRate, 4);
    file.read((char*)&wavHeader.fmtBlockAlign, 2);
    file.read((char*)&wavHeader.fmtBitsPerSample, 2);

    int64_t tmpPos = file.tellg();
    file.read(wavHeader.dataID, 4);
    if(std::strcmp(wavHeader.dataID, "data")!=0) {
        file.seekg(tmpPos);
        file.read((char*)&wavHeader.fmtExtraParamSize, 2);
        wavHeader.fmtExtraParams = new char(wavHeader.fmtExtraParamSize);
        file.read(wavHeader.fmtExtraParams, wavHeader.fmtExtraParamSize);
        file.read(wavHeader.dataID, 4);
    } else {
        wavHeader.fmtExtraParamSize = 0;
        wavHeader.fmtExtraParams = nullptr;
    }

    if(std::strcmp(wavHeader.dataID, "data")!=0) {
        return false;
    }
    file.read((char*)&wavHeader.dataSize, 4);

    return true;
}

std::string File::getName() {
    return (name + extension);
}

bool File::isExist(const std::string& filename) {
    std::ifstream f(filename.c_str());
    return f.good();
}

bool File::splitExtension(const std::string& fileName) {
    extension = "";
    std::string::size_type idx;
    idx = fileName.rfind('.');

    if(idx != std::string::npos)
    {
        extension = fileName.substr(idx+1);
        name = fileName.substr(0, idx);
    }

    return true;
}

bool File::generateName() {
    std::string fileName = name;
    uint32_t i = 0;

    while(isExist(fileName+'.'+extension)) {
        fileName = name+'('+std::to_string(i)+')';
        i++;
    }
    name = fileName;

    return true;
}

int File::write(uint16_t value) {
    little_endian::write<uint16_t>(stream, value);

    return 0;
};

bool File::read(Buffer& buffer)
{

}

bool File::write(Buffer buffer) {
    buffer.read(stream);
};


bool File::isAvailable()
{
    return true;
}

bool File::write(std::vector<uint8_t> arr) {
    for(int i=0; i<arr.size(); i++) {
        little_endian::write<uint8_t>(stream, arr[i]);
    }

    return true;
};

int File::finalize() {
    // (We'll need the final file size to fix the chunk sizes above)
    posFileLength = stream.tellp();

    // Fix the data chunk header to contain the data size
    stream.seekp(posDataChunk + 4);
    little_endian::write<uint32_t>(stream, (uint32_t)(posFileLength - posDataChunk + 8));

    // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    stream.seekp(0 + 4);
    little_endian::write<uint32_t>(stream, (uint32_t)(posFileLength - 8));

    stream.close();
    finalized = true;

    return 0;
}
