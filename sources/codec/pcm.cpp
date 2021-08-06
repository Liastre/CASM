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
    dataStream.read<std::array<char, 4>>(wavHeader.chunkID);
    dataStream.read<std::uint32_t>(wavHeader.chunkSize);
    dataStream.read<std::array<char, 4>>(wavHeader.chunkFormat);
    dataStream.read<std::array<char, 4>>(wavHeader.fmtID);
    dataStream.read<std::uint32_t>(wavHeader.fmtSize);
    dataStream.read<std::uint16_t>(wavHeader.fmtAudioFormat);
    dataStream.read<std::uint16_t>(wavHeader.fmtNumChannels);
    dataStream.read<std::uint32_t>(wavHeader.fmtSampleRate);
    dataStream.read<std::uint32_t>(wavHeader.fmtByteRate);
    dataStream.read<std::uint16_t>(wavHeader.fmtBlockAlign);
    dataStream.read<std::uint16_t>(wavHeader.fmtBitsPerSample);

    _posDataChunk = dataStream.tellPos();
    dataStream.read<std::array<char, 4>>(wavHeader.dataID);
    std::array<char, 4> dataID = { 'd', 'a', 't', 'a' };
    if (wavHeader.dataID != dataID) {
        dataStream.seekPos(_posDataChunk);
        dataStream.read<uint16_t>(wavHeader.fmtExtraParamSize);
        if (wavHeader.fmtExtraParamSize > 0) {
            // TODO: fix memory leak
            wavHeader.fmtExtraParams = new char(wavHeader.fmtExtraParamSize);
            dataStream.read<char*>(wavHeader.fmtExtraParams, wavHeader.fmtExtraParamSize);
        }
        dataStream.read<std::array<char, 4>>(wavHeader.dataID);
    }
    if (wavHeader.dataID != dataID) {
        // TODO: add result
        return WaveProperties();
    }
    dataStream.read<uint32_t>(wavHeader.dataSize);

    // TODO: set bits type depending on wavHeader.fmtAudioFormat
    return WaveProperties(
      wavHeader.fmtNumChannels,
      wavHeader.fmtSampleRate,
      wavHeader.fmtBitsPerSample,
      wavHeader.fmtBitsPerSample,
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

    finalized = true;

    return true;
}

} // namespace Codec

} // namespace CASM
