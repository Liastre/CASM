#include "CASM/codec/pcm.hpp"

namespace CASM {

namespace Codec {

BufferStatus
Pcm::readData(DataStream& dataStream, Buffer& buffer) {
    return buffer.write(dataStream);
}

bool
Pcm::writeData(DataStream& dataStream, Buffer const& buffer) {
    buffer.read(dataStream);
    return true;
}

WaveProperties
Pcm::readHeader(DataStream& dataStream) {
    dataStream.read<std::array<char, 4>>(_wavHeader.chunkID);
    dataStream.read<std::uint32_t>(_wavHeader.chunkSize);
    dataStream.read<std::array<char, 4>>(_wavHeader.chunkFormat);
    dataStream.read<std::array<char, 4>>(_wavHeader.fmtID);
    dataStream.read<std::uint32_t>(_wavHeader.fmtSize);
    dataStream.read<std::uint16_t>(_wavHeader.fmtAudioFormat);
    dataStream.read<std::uint16_t>(_wavHeader.fmtNumChannels);
    dataStream.read<std::uint32_t>(_wavHeader.fmtSampleRate);
    dataStream.read<std::uint32_t>(_wavHeader.fmtByteRate);
    dataStream.read<std::uint16_t>(_wavHeader.fmtBlockAlign);
    dataStream.read<std::uint16_t>(_wavHeader.fmtBitsPerSample);

    _posDataChunk = dataStream.tellPos();
    dataStream.read<std::array<char, 4>>(_wavHeader.dataID);
    std::array<char, 4> dataID = { 'd', 'a', 't', 'a' };
    if (_wavHeader.dataID != dataID) {
        dataStream.seekPos(_posDataChunk);
        dataStream.read<uint16_t>(_wavHeader.fmtExtraParamSize);
        if (_wavHeader.fmtExtraParamSize > 0) {
            // TODO: fix memory leak
            _wavHeader.fmtExtraParams = new char(_wavHeader.fmtExtraParamSize);
            dataStream.read<char*>(_wavHeader.fmtExtraParams, _wavHeader.fmtExtraParamSize);
        }
        dataStream.read<std::array<char, 4>>(_wavHeader.dataID);
    }
    if (_wavHeader.dataID != dataID) {
        // TODO: add result
        return WaveProperties();
    }
    dataStream.read<uint32_t>(_wavHeader.dataSize);

    // TODO: set bits type depending on _wavHeader.fmtAudioFormat
    return WaveProperties(
      _wavHeader.fmtNumChannels,
      _wavHeader.fmtSampleRate,
      _wavHeader.fmtBitsPerSample,
      _wavHeader.fmtBitsPerSample,
      true);
}

bool
Pcm::writeHeader(DataStream& dataStream, WaveProperties const& waveProperties) {
    dataStream.write<char[4]>({ 'R', 'I', 'F', 'F' });
    dataStream.write<std::uint32_t>(0);
    dataStream.write<char[4]>({ 'W', 'A', 'V', 'E' });
    dataStream.write<char[4]>({ 'f', 'm', 't', ' ' });
    dataStream.write<std::uint32_t>(16);
    dataStream.write<std::uint16_t>(3);
    dataStream.write<std::uint16_t>(waveProperties.getChannelsCount());
    dataStream.write<std::uint32_t>(waveProperties.getSamplesPerSecond());
    dataStream.write<std::uint32_t>(waveProperties.getBytesPerSecond());
    dataStream.write<std::uint16_t>(waveProperties.getBlockAlign());
    dataStream.write<std::uint16_t>(waveProperties.getBitsPerSample());

    // Write the data chunk header
    _posDataChunk = dataStream.tellPos();
    // (chunk size to be filled in later)
    dataStream.write("data----", 8);

    return true;
}

bool
Pcm::finalize(DataStream& dataStream) {
    // We'll need the final file size to fix the chunk sizes above
    _posFileLength = dataStream.tellPos();
    // Fix the data chunk header to contain the data size
    dataStream.seekPos(_posDataChunk + 4);
    dataStream.write<uint32_t>((uint32_t)(_posFileLength - _posDataChunk + 8));
    // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    dataStream.seekPos(0 + 4);
    dataStream.write<uint32_t>((uint32_t)(_posFileLength - 8));
    _isFinalized = true;

    return true;
}

} // namespace Codec

} // namespace CASM
