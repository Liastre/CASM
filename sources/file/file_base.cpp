#include <CASM/core/file_base.hpp>


namespace CASM {

Buffer FileBase::openCaptureStream(std::chrono::duration< double > bufferDuration) {
    if (readHeader()) {
        return Buffer(_streamWaveProperties, bufferDuration);
    } else {
        return Buffer();
    }
}


bool FileBase::openRenderStream(Buffer buffer) {
    _streamWaveProperties = buffer.getWaveProperties();

    return writeHeader();
}


void FileBase::closeCaptureStream() {
    _stream->close();
}


void FileBase::closeRenderStream() {
    finalize();
    _stream->close();
}


std::string FileBase::getName() const {
    return (_name+_extension);
}


bool FileBase::isAvailable() const {
    return _stream->good();
}

} // namespace CASM
