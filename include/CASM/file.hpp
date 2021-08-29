/**
 * Generic File endpoint implementation
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include "types.hpp"
#include "core/end_point.hpp"
#include "core/buffer.hpp"
#include "codec/codec.hpp"
#include "data_stream/fstream.hpp"
#include "utility/windows_utilities.hpp"
#include <string>
#include <fstream>

namespace CASM {

/**
 * File wrapper, wraps existing or
 * user defined File type class
 */
class File final : public EndPointBase {
public:
    File() = default;
    template <class TCodec, class TDataStream>
    File(TCodec&& codec, TDataStream&& dataStream, std::string const& path, bool doCreateNewFileOnWrite = true) {
        _doCreateNewFileOnWrite = doCreateNewFileOnWrite;
        std::string resPath(path);
        _formatPath(resPath);
        if (!_parsePath(resPath)) {
            // log invalid path
        }
        _isValid = { true };
        _codec = std::make_shared<TCodec>(std::move(codec));
        _dataStream = std::make_shared<TDataStream>(std::move(dataStream));
    };
    ~File();

    // FileInterface
    bool openCaptureStream(Duration const& bufferDuration, Buffer& buffer) final;
    bool openRenderStream(Buffer const& buffer) final;
    void closeRenderStream() final;
    void closeCaptureStream() final;
    BufferStatus read(Buffer& buffer) final;
    bool write(Buffer const& buffer) final;
    bool isAvailable() const final;
    std::string getName() const;

    operator bool() const;

private:
    bool _isExist() const;
    void _formatPath(std::string& path);
    bool _parsePath(std::string const& path);
    void _generateName();

private:
    std::string _path;
    std::string _destination;
    std::string _name;
    std::string _extension;
    bool _doCreateNewFileOnWrite = false;
    std::shared_ptr<Codec::CodecInterface> _codec;
    std::shared_ptr<DataStream::DataStreamInterface> _dataStream;
};

} // namespace CASM
