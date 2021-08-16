/**
 * Generic File endpoint implementation
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include <CASM/types.hpp>
#include <CASM/core/file_base.hpp>
#include <CASM/core/buffer.hpp>
#include <CASM/codec/codec.hpp>
#include <CASM/data_stream/fstream.hpp>
#include <CASM/utility/windows_utilities.hpp>
#include <string>
#include <fstream>

namespace CASM {

/**
 * File wrapper, wraps existing or
 * user defined File type class
 */
template <class TCodec, class TDataStream = DataStream::Fstream>
class File final : public EndPointBase {
public:
    File() = default;
    File(std::string const& path, bool doCreateNewFileOnWrite = true);
    ~File() override;

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
    std::shared_ptr<DataStream::DataStreamInterface> _file;
    // TODO: replace with library
    char kSplitPathSymbol = '/';
};

template <class TCodec, class TDataStream>
File<TCodec, TDataStream>::File(std::string const& path, bool doCreateNewFileOnWrite) {
    _doCreateNewFileOnWrite = doCreateNewFileOnWrite;
    std::string resPath(path);
    _formatPath(resPath);
    if (!_parsePath(resPath)) {
        // log invalid path
    }
    _isValid = { true };
};

template <class TCodec, class TDataStream>
File<TCodec, TDataStream>::~File() {
    _codec.reset();
}

template <class TCodec, class TDataStream>
bool // TODO: redo with result
File<TCodec, TDataStream>::openCaptureStream(Duration const& bufferDuration, Buffer& buffer) {
    if (!_isExist()) {
        return false;
    }

    _codec = std::make_shared<TCodec>();
    _file = std::make_shared<TDataStream>(_path);
    _file->open(Access::READ);
    _streamWaveProperties = _codec->readHeader(*_file);
    buffer = Buffer(_streamWaveProperties, bufferDuration);
    // TODO: add readHeader check
    return true;
}

template <class TCodec, class TDataStream>
bool
File<TCodec, TDataStream>::openRenderStream(Buffer const& buffer) {
    if (_isExist() && _doCreateNewFileOnWrite) {
        _generateName();
    }

    _codec = std::make_shared<TCodec>();
    _file = std::make_shared<TDataStream>(_path);
    if (!_file->open(Access::WRITE)) {
        return false;
    }

    _streamWaveProperties = buffer.getWaveProperties();
    return _codec->writeHeader(*_file, _streamWaveProperties);
}

template <class TCodec, class TDataStream>
void
File<TCodec, TDataStream>::closeRenderStream() {
    _codec->finalize(*_file);
    _file->close();
}

template <class TCodec, class TDataStream>
void
File<TCodec, TDataStream>::closeCaptureStream() {
    _file->close();
}

template <class TCodec, class TDataStream>
BufferStatus
File<TCodec, TDataStream>::read(Buffer& buffer) {
    return _codec->readData(*_file, buffer);
}

template <class TCodec, class TDataStream>
bool
File<TCodec, TDataStream>::write(Buffer const& buffer) {
    return _codec->writeData(*_file, buffer);
}

template <class TCodec, class TDataStream>
std::string
File<TCodec, TDataStream>::getName() const {
    return _name;
}

template <class TCodec, class TDataStream>
bool
File<TCodec, TDataStream>::isAvailable() const {
    return _isExist();
}

template <class TCodec, class TDataStream>
bool
File<TCodec, TDataStream>::_isExist() const {
    return Util::Filesystem::isExist(_path);
}

template <class TCodec, class TDataStream>
void
File<TCodec, TDataStream>::_formatPath(std::string& path) {
    std::replace(path.begin(), path.end(), '\\', kSplitPathSymbol);
}

template <class TCodec, class TDataStream>
bool
File<TCodec, TDataStream>::_parsePath(std::string const& path) {
    std::string::size_type extensionIndex = path.rfind('.');
    std::string::size_type destinationIndex = path.rfind(kSplitPathSymbol);
    if (destinationIndex != std::string::npos) {
        _destination = path.substr(0, destinationIndex);
        ++destinationIndex; // escape split character
    } else {
        destinationIndex = 0;
    }

    _name = path.substr(destinationIndex, extensionIndex - destinationIndex);
    _extension = path.substr(extensionIndex);
    _path = path;

    return true;
}

template <class TCodec, class TDataStream>
void
File<TCodec, TDataStream>::_generateName() {
    std::string tmpName;
    uint32_t i(0);
    while (_isExist()) {
        tmpName.clear();
        tmpName.append(_name).append("(").append(std::to_string(i)).append(")");
        _path.clear();
        _path.append(_destination).append(tmpName).append(_extension);
        i++;
    }
    _name = tmpName;
}

template <class TCodec, class TDataStream>
File<TCodec, TDataStream>::operator bool() const {
    return isValid();
}

template <class C>
struct IsFile : std::false_type { };

template <class C>
struct IsFile<File<C>> : std::true_type { };

} // namespace CASM
