// =============== DESCRIPTION ===============
// Created on 30 May.
// ===========================================

#include "CASM/file.hpp"

#include <cstdint>
#include <cstring>
#include <cassert>
#include <CASM/CASM.hpp>

namespace little_endian {
template<typename T>
void write(std::fstream& stream, const T& t) {
    stream.write((const char*) &t, sizeof(T));
}

template<typename T>
void read(std::fstream& stream, const T& t) {
    stream.read((char*) &t, sizeof(T));
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

File::File(std::string fileName, WaveProperties waveProperties) {
    posDataChunk = 0;
    posFileLength = 0;
    finalized = false;
    splitExtension(fileName);
    File::streamWaveProperties = waveProperties;
}

File::~File() {
    if (!finalized) {
        close();
    }
}

void File::open(CASM::Access access)
{
    switch(access){
    case CASM::WRITE:
        generateName();
        path = name + '.' + extension;
        writeHeader();
        break;
    case CASM::READ:
        path = name + '.' + extension;
        if(isExist(path)) {
            readHeader();
        }
        break;
    default:
        throw std::runtime_error("Impossible access value");
    }
}

bool File::writeHeader() {
    stream.open(path, std::ios::out | std::ios::binary);

    little_endian::write<char[4]>(stream, {'R','I','F','F'});   // RIFF chunk
    little_endian::write<uint32_t>(stream, 0);                  // RIFF chunk size in bytes
    little_endian::write<char[4]>(stream, {'W','A','V','E'});   // file type

    little_endian::write<char[4]>(stream, {'f','m','t',' '});   // fmt chunk
    little_endian::write<uint32_t>(stream, 16);                 // size of fmt chunk 16 + extra format bytes
    little_endian::write<uint16_t>(stream, 3);                  // format (compression code)

    little_endian::write<uint16_t>(stream, streamWaveProperties.getChannelsCount());
    little_endian::write<uint32_t>(stream, streamWaveProperties.getSamplesPerSecond());
    little_endian::write<uint32_t>(stream, streamWaveProperties.getBytesPerSecond());
    little_endian::write<uint16_t>(stream, streamWaveProperties.getBlockAlign());
    little_endian::write<uint16_t>(stream, streamWaveProperties.getBitsPerSample());

    // Write the data chunk header
    posDataChunk = stream.tellp();
    stream << "data----";  // (chunk size to be filled in later)

    return true;
}

bool File::readHeader() {
    stream.open(path, std::ios::in | std::ios::binary);

    little_endian::read<char[4]>(stream, wavHeader.chunkID);
    little_endian::read<uint32_t>(stream, wavHeader.chunkSize);
    little_endian::read<char[4]>(stream, wavHeader.chunkFormat);
    little_endian::read<char[4]>(stream, wavHeader.fmtID);
    little_endian::read<uint32_t>(stream, wavHeader.fmtSize);
    little_endian::read<uint16_t>(stream, wavHeader.fmtAudioFormat);
    little_endian::read<uint16_t>(stream, wavHeader.fmtNumChannels);
    little_endian::read<uint32_t>(stream, wavHeader.fmtSampleRate);
    little_endian::read<uint32_t>(stream, wavHeader.fmtByteRate);
    little_endian::read<uint16_t>(stream, wavHeader.fmtBlockAlign);
    little_endian::read<uint16_t>(stream, wavHeader.fmtBitsPerSample);

    int64_t tmpPos = stream.tellg();
    little_endian::read<char[4]>(stream, wavHeader.dataID);
    if(std::strcmp(wavHeader.dataID, "data")!=0) {
        stream.seekg(tmpPos);
        little_endian::read<uint16_t>(stream, wavHeader.fmtExtraParamSize);
        wavHeader.fmtExtraParams = new char(wavHeader.fmtExtraParamSize);
        // TODO something
        stream.read(wavHeader.fmtExtraParams, wavHeader.fmtExtraParamSize);
        little_endian::read<char[4]>(stream, wavHeader.dataID);
    } else {
        wavHeader.fmtExtraParamSize = 0;
        wavHeader.fmtExtraParams = nullptr;
    }

    if(std::strcmp(wavHeader.dataID, "data")!=0) {
        return false;
    }
    little_endian::read<uint32_t>(stream, wavHeader.dataSize);

    // TODO: set bits type depending on wavHeader.fmtAudioFormat
    streamWaveProperties = WaveProperties(wavHeader.fmtNumChannels, wavHeader.fmtSampleRate, PCM_16BIT_SIGNED);

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

bool File::read(Buffer& buffer) {

}

bool File::write(Buffer buffer) {
    buffer.read(stream);
};


bool File::isAvailable()
{
    return true;
}

void File::close()
{
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
}
