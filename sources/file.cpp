#include "CASM/file.hpp"
#include "file/file_wave.hpp"
#include <cstdint>
#include <cstring>
#include <algorithm>

namespace {

constexpr char kSplitPathSymbol = '/';

}

namespace CASM {

File::File(std::string const& path, bool shouldForceWriting) {
    std::string resPath(path);
    _formatPath(resPath);
    if (_parsePath(resPath)) {
        _shouldForceWriting = shouldForceWriting;
        _file = std::make_shared<FileWave>(_path);
    } else {
        // report no extension
    }
}

File::~File() {
    _file.reset();
}

bool
File::openCaptureStream(Duration const& bufferDuration, Buffer& buffer) {
    if (!_isExist()) {
        return false;
    }

    return _file->openCaptureStream(bufferDuration, buffer);
}

bool
File::openRenderStream(Buffer const& buffer) {
    if (_isExist() && !_shouldForceWriting) {
        _generateName();
    }
    return _file->openRenderStream(buffer);
}

void
File::closeRenderStream() {
    _file->closeRenderStream();
}

void
File::closeCaptureStream() {
    _file->closeCaptureStream();
}

BufferStatus
File::read(Buffer& buffer) {
    return _file->read(buffer);
}

bool
File::write(Buffer const& buffer) {
    return _file->write(buffer);
}

bool
File::readHeader() {
    return _file->readHeader();
}

bool
File::writeHeader() {
    return _file->writeHeader();
}

bool
File::isAvailable() const {
    return _file->isAvailable();
}

bool
File::_isExist() const {
    std::ifstream file(_path);
    return file.good();
}

void
File::_formatPath(std::string& path) {
    std::replace(path.begin(), path.end(), '\\', kSplitPathSymbol);
}

bool
File::_parsePath(std::string const& path) {
    std::string::size_type extensionIndex = path.rfind('.');
    if (extensionIndex == std::string::npos) {
        // TODO: logger message no extension
        return false;
    }

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
        tmpName = _name + '(' + std::to_string(i) + ')';
        _path = _destination + tmpName + _extension;
        i++;
    }
    _name = tmpName;
    _file->setPath(_path);
}

bool
File::finalize() {
    return _file->finalize();
}

void
File::setPath(std::string const& path) {
    // TODO: implementation
}

WaveProperties
File::getStreamWaveProperties() const {
    return _file->getStreamWaveProperties();
}

bool
File::isInUsage() const {
    return _file->isInUsage();
}

std::string
File::getName() const {
    return _name;
}

File::operator bool() const {
    return _isExist();
}

bool
File::isValid() const {
    return _file->isValid();
}

} // namespace CASM