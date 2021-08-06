#pragma once

#include <CASM/core/Buffer.hpp>
#include <CASM/data_stream/data_stream.hpp>

namespace CASM {

namespace Codec {

class CodecInterface {
public:
    using DataStream = DataStream::DataStreamInterface;

public:
    virtual ~CodecInterface() = default;

    virtual WaveProperties readHeader(DataStream&) = 0;
    virtual bool writeHeader(DataStream&, WaveProperties const&) = 0;
    virtual BufferStatus readData(DataStream&, Buffer&) = 0;
    virtual bool writeData(DataStream&, Buffer const&) = 0;
    virtual bool finalize(DataStream&) = 0;
};

} // namespace Codec

} // namespace CASM
