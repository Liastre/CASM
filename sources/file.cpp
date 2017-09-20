// =============== DESCRIPTION ===============
// Created on 30 May.
// ===========================================

#include "CASM/file.hpp"

#include <cstdint>
#include <cstring>
#include <cassert>
#include <CASM/CASM.hpp>


namespace little_endian {
template < typename T >
void write(std::fstream &stream, const T &t) {
    stream.write((const char *) &t, sizeof(T));
}


template < typename T >
void read(std::fstream &stream, const T &t) {
    stream.read((char *) &t, sizeof(T));
}
}

// TODO: remake
namespace big_endian {
template < typename Word >
std::ostream &write(std::ostream &outs, Word value, unsigned size = sizeof(Word)) {
    for (; size; --size, value >>= 8)
        outs.put(static_cast <char> (value & 0xFF));
    return outs;
}
}

namespace CASM {

File::File() {
    name = path = extension = "";
    stream = new std::fstream();
}


File::File(std::string fileName) :File() {
    posDataChunk = 0;
    posFileLength = 0;
    finalized = false;
    path = fileName;
    splitExtension(fileName);
}


File::~File() {
    // TODO: check if fstream exist, or create base class
    if (!finalized) {
        //close();
    }
}


Buffer File::open(std::chrono::duration< double > duration) {
    if (!isExist(path)) {
        return (Buffer());
    }
    readHeader();

    return Buffer(streamWaveProperties, duration);
}


bool File::open(Buffer buffer) {
    streamWaveProperties = buffer.getWaveProperties();

    generateName();
    path = name+'.'+extension;
    return writeHeader();
}


void File::close() {
    // (We'll need the final file size to fix the chunk sizes above)
    posFileLength = stream->tellp();

    // Fix the data chunk header to contain the data size
    stream->seekp(posDataChunk+4);
    little_endian::write< uint32_t >(*stream, (uint32_t) (posFileLength-posDataChunk+8));

    // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    stream->seekp(0+4);
    little_endian::write< uint32_t >(*stream, (uint32_t) (posFileLength-8));

    stream->close();
    finalized = true;
}


bool File::read(Buffer &buffer) {
    buffer.write(*stream);
}


bool File::write(Buffer buffer) {
    buffer.read(*stream);
};


bool File::readHeader() {
    stream->open(path, std::ios::in | std::ios::binary);

    little_endian::read< std::array< char, 4 > >(*stream, wavHeader.chunkID);
    little_endian::read< uint32_t >(*stream, wavHeader.chunkSize);
    little_endian::read< std::array< char, 4 > >(*stream, wavHeader.chunkFormat);
    little_endian::read< std::array< char, 4 > >(*stream, wavHeader.fmtID);
    little_endian::read< uint32_t >(*stream, wavHeader.fmtSize);
    little_endian::read< uint16_t >(*stream, wavHeader.fmtAudioFormat);
    little_endian::read< uint16_t >(*stream, wavHeader.fmtNumChannels);
    little_endian::read< uint32_t >(*stream, wavHeader.fmtSampleRate);
    little_endian::read< uint32_t >(*stream, wavHeader.fmtByteRate);
    little_endian::read< uint16_t >(*stream, wavHeader.fmtBlockAlign);
    little_endian::read< uint16_t >(*stream, wavHeader.fmtBitsPerSample);
    int64_t tmpPos = stream->tellg();
    little_endian::read< std::array< char, 4 > >(*stream, wavHeader.dataID);

    std::array< char, 4 > dataID = {'d', 'a', 't', 'a'};
    if (wavHeader.dataID!=dataID) {
        stream->seekg(tmpPos);
        little_endian::read< uint16_t >(*stream, wavHeader.fmtExtraParamSize);
        wavHeader.fmtExtraParams = new char(wavHeader.fmtExtraParamSize);
        // TODO something
        stream->read(wavHeader.fmtExtraParams, wavHeader.fmtExtraParamSize);
        little_endian::read< std::array< char, 4 > >(*stream, wavHeader.dataID);
    }
    else {
        wavHeader.fmtExtraParamSize = 0;
        wavHeader.fmtExtraParams = nullptr;
    }

    if (wavHeader.dataID!=dataID) {
        return false;
    }
    little_endian::read< uint32_t >(*stream, wavHeader.dataSize);

    // TODO: set bits type depending on wavHeader.fmtAudioFormat
    streamWaveProperties = WaveProperties(wavHeader.fmtNumChannels, wavHeader.fmtSampleRate, PCM_32BIT_SIGNED);

    return true;
}


bool File::writeHeader() {
    stream->open(path, std::ios::out | std::ios::binary);

    little_endian::write< char[4] >(*stream, {'R', 'I', 'F', 'F'});   // RIFF chunk
    little_endian::write< uint32_t >(*stream, 0);                  // RIFF chunk size in bytes
    little_endian::write< char[4] >(*stream, {'W', 'A', 'V', 'E'});   // file type

    little_endian::write< char[4] >(*stream, {'f', 'm', 't', ' '});   // fmt chunk
    little_endian::write< uint32_t >(*stream, 16);                 // size of fmt chunk 16 + extra format bytes
    little_endian::write< uint16_t >(*stream, 3);                  // format (compression code)

    little_endian::write< uint16_t >(*stream, streamWaveProperties.getChannelsCount());
    little_endian::write< uint32_t >(*stream, streamWaveProperties.getSamplesPerSecond());
    little_endian::write< uint32_t >(*stream, streamWaveProperties.getBytesPerSecond());
    little_endian::write< uint16_t >(*stream, streamWaveProperties.getBlockAlign());
    little_endian::write< uint16_t >(*stream, streamWaveProperties.getBitsPerSample());

    // Write the data chunk header
    posDataChunk = stream->tellp();
    *stream << "data----";  // (chunk size to be filled in later)

    return true;
}


std::string File::getName() {
    return (name+extension);
}


bool File::isExist(const std::string &filename) {
    std::ifstream f(filename.c_str());
    return f.good();
}


bool File::splitExtension(const std::string &fileName) {
    extension = "";
    std::string::size_type idx;
    idx = fileName.rfind('.');

    if (idx!=std::string::npos) {
        extension = fileName.substr(idx+1);
        name = fileName.substr(0, idx);
    }

    return true;
}


bool File::generateName() {
    std::string fileName = name;
    uint32_t i = 0;

    while (isExist(fileName+'.'+extension)) {
        fileName = name+'('+std::to_string(i)+')';
        i++;
    }
    name = fileName;

    return true;
}


bool File::isAvailable() {
    return true;
}

}