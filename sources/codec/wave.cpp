#include "CASM/codec/wave.hpp"

namespace CASM {

namespace Codec {

Wave::Wave(std::string const& filePath) {
    _path = filePath;
}

bool
Wave::open(Access access) {
    switch (access) {
    case Access::WRITE:
        _stream->open(_path, std::ios::out | std::ios::binary);
        break;
    case Access::READ:
        _stream->open(_path, std::ios::in | std::ios::binary);
        break;
    default:
        return false;
    }

    return true;
}

bool
Wave::close() {
    _stream->close();
    return !_stream->good();
}

bool
Wave::isGood() const {
    return _stream->good();
}

BufferStatus
Wave::readData(Buffer& buffer) {
    return buffer.write(*_stream);
}

bool
Wave::writeData(Buffer const& buffer) {
    buffer.read(*_stream);
    return true;
}

WaveProperties
Wave::readHeader() {
    little_endian::read<std::array<char, 4>>(*_stream, wavHeader.chunkID); // RIFF chunk
    little_endian::read<uint32_t>(*_stream, wavHeader.chunkSize); // RIFF chunk size in bytes
    little_endian::read<std::array<char, 4>>(*_stream, wavHeader.chunkFormat); // file type

    little_endian::read<std::array<char, 4>>(*_stream, wavHeader.fmtID); // fmt chunk
    little_endian::read<uint32_t>(*_stream, wavHeader.fmtSize); // size of fmt chunk 16 + extra format bytes
    little_endian::read<uint16_t>(*_stream, wavHeader.fmtAudioFormat); // format (compression code)

    little_endian::read<uint16_t>(*_stream, wavHeader.fmtNumChannels);
    little_endian::read<uint32_t>(*_stream, wavHeader.fmtSampleRate);
    little_endian::read<uint32_t>(*_stream, wavHeader.fmtByteRate);
    little_endian::read<uint16_t>(*_stream, wavHeader.fmtBlockAlign);
    little_endian::read<uint16_t>(*_stream, wavHeader.fmtBitsPerSample);
    int64_t tmpPos = _stream->tellg();
    little_endian::read<std::array<char, 4>>(*_stream, wavHeader.dataID);

    std::array<char, 4> dataID = { 'd', 'a', 't', 'a' };
    if (wavHeader.dataID != dataID) {
        _stream->seekg(tmpPos);
        little_endian::read<uint16_t>(*_stream, wavHeader.fmtExtraParamSize);
        if (wavHeader.fmtExtraParamSize > 0) {
            // TODO: fix memory leak
            wavHeader.fmtExtraParams = new char(wavHeader.fmtExtraParamSize);
            little_endian::read<char*>(*_stream, wavHeader.fmtExtraParams, wavHeader.fmtExtraParamSize);
        }
        little_endian::read<std::array<char, 4>>(*_stream, wavHeader.dataID);
    }
    if (wavHeader.dataID != dataID) {
        // TODO: add result
        return WaveProperties();
    }
    little_endian::read<uint32_t>(*_stream, wavHeader.dataSize);

    // TODO: set bits type depending on wavHeader.fmtAudioFormat
    return WaveProperties(
      wavHeader.fmtNumChannels,
      wavHeader.fmtSampleRate,
      wavHeader.fmtBitsPerSample,
      wavHeader.fmtBitsPerSample,
      true);
}

bool
Wave::writeHeader(WaveProperties const& waveProperties) {
    little_endian::write<char[4]>(*_stream, { 'R', 'I', 'F', 'F' }); // RIFF chunk
    little_endian::write<uint32_t>(*_stream, 0); // RIFF chunk size in bytes
    little_endian::write<char[4]>(*_stream, { 'W', 'A', 'V', 'E' }); // file type

    little_endian::write<char[4]>(*_stream, { 'f', 'm', 't', ' ' }); // fmt chunk
    little_endian::write<uint32_t>(*_stream, 16); // size of fmt chunk 16 + extra format bytes
    little_endian::write<uint16_t>(*_stream, 3); // format (compression code)

    little_endian::write<uint16_t>(*_stream, waveProperties.getChannelsCount());
    little_endian::write<uint32_t>(*_stream, waveProperties.getSamplesPerSecond());
    little_endian::write<uint32_t>(*_stream, waveProperties.getBytesPerSecond());
    little_endian::write<uint16_t>(*_stream, waveProperties.getBlockAlign());
    little_endian::write<uint16_t>(*_stream, waveProperties.getBitsPerSample());

    // Write the data chunk header
    posDataChunk = _stream->tellp();
    *_stream << "data----"; // (chunk size to be filled in later)

    return true;
}

bool
Wave::finalize() {
    // (We'll need the final file size to fix the chunk sizes above)
    posFileLength = _stream->tellp();

    // Fix the data chunk header to contain the data size
    _stream->seekp(posDataChunk + 4);
    little_endian::write<uint32_t>(*_stream, (uint32_t)(posFileLength - posDataChunk + 8));

    // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    _stream->seekp(0 + 4);
    little_endian::write<uint32_t>(*_stream, (uint32_t)(posFileLength - 8));

    finalized = true;

    return true;
}

} // namespace Codec

} // namespace CASM
