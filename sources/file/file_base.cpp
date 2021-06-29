#include <CASM/core/file_base.hpp>

namespace CASM {

FileBase::FileBase(std::string const& filePath) {
    _path = filePath;
}

bool
FileBase::openCaptureStream(Duration const& bufferDuration, Buffer& buffer) {
    _stream->open(_path, std::ios::in | std::ios::binary);
    if (readHeader()) {
        buffer = Buffer(_streamWaveProperties, bufferDuration);
        return true;
    }

    return false;
}

bool
FileBase::openRenderStream(Buffer const& buffer) {
    _stream->open(_path, std::ios::out | std::ios::binary);
    _streamWaveProperties = buffer.getWaveProperties();

    return writeHeader();
}

void
FileBase::closeCaptureStream() {
    _stream->close();
}

void
FileBase::closeRenderStream() {
    finalize();
    _stream->close();
}

bool
FileBase::isAvailable() const {
    return _stream->good();
}

void
FileBase::setPath(std::string const& path) {
    _path = path;
}

} // namespace CASM
