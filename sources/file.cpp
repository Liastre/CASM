#include "CASM/file.hpp"
#include <algorithm>

namespace {

// TODO: replace with library
constexpr char kSplitPathSymbol = '/';

}

namespace CASM {

File::~File() {
    _codec.reset();
}

bool // TODO: redo with result
File::openCaptureStream(Duration const& bufferDuration, Buffer& buffer) {
    if (!_isExist()) {
        return false;
    }

    if (!_dataStream->open(Access::READ, _path)) {
        return false;
    }

    _streamWaveProperties = _codec->readHeader(*_dataStream);
    buffer = Buffer(_streamWaveProperties, bufferDuration);
    // TODO: add readHeader check
    return true;
}

bool
File::openRenderStream(Buffer const& buffer) {
    if (_isExist() && _doCreateNewFileOnWrite) {
        _generateName();
    }

    if (!_dataStream->open(Access::WRITE, _path)) {
        return false;
    }

    _streamWaveProperties = buffer.getWaveProperties();
    return _codec->writeHeader(*_dataStream, _streamWaveProperties);
}

void
File::closeRenderStream() {
    _codec->finalize(*_dataStream);
    _dataStream->close();
}

void
File::closeCaptureStream() {
    _dataStream->close();
}

BufferStatus
File::read(Buffer& buffer) {
    return _codec->readData(*_dataStream, buffer);
}

bool
File::write(Buffer const& buffer) {
    return _codec->writeData(*_dataStream, buffer);
}

std::string
File::getName() const {
    return _name;
}

bool
File::isAvailable() const {
    return _isExist();
}

bool
File::_isExist() const {
    return Util::Filesystem::isExist(_path);
}

void
File::_formatPath(std::string& path) {
    std::replace(path.begin(), path.end(), '\\', kSplitPathSymbol);
}

bool
File::_parsePath(std::string const& path) {
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

void
File::_generateName() {
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

File::operator bool() const {
    return isValid();
}

} // namespace CASM