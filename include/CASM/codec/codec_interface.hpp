#include <CASM/core/Buffer.hpp>

namespace CASM {

namespace Codec {

class CodecInterface {
public:
    virtual ~CodecInterface() = default;

    virtual bool open(Access access) = 0;
    virtual bool close() = 0;
    virtual WaveProperties readHeader() = 0;
    virtual bool writeHeader(WaveProperties const& waveProperties) = 0;
    virtual BufferStatus readData(Buffer& buffer) = 0;
    virtual bool writeData(Buffer const& buffer) = 0;
    virtual bool finalize() = 0;
    virtual bool isGood() const = 0;
};

} // namespace Codec

} // namespace CASM
