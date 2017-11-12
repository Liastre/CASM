/// @file file.cpp
/// @brief definition of File class

#include "CASM/file.hpp"
#include "file/file_wave.hpp"

#include <cstdint>
#include <cstring>
#include <algorithm>


namespace CASM {

File::File(std::string path, bool isForceWriting) {
    if(parsePath(path)) {
        if(!isForceWriting) {
            generateName();
        }

        _path = _destination+_name+_extension;
        _file = std::make_shared< FileWave >(_path);
    } else {
        // report no extension
    }
}


File::~File() {
    _file.reset();
}


Buffer File::openCaptureStream(std::chrono::duration< double > bufferDuration) {
    return _file->openCaptureStream(bufferDuration);
}


bool File::openRenderStream(Buffer buffer) {
    return _file->openRenderStream(buffer);
}


void File::closeRenderStream() {
    _file->closeRenderStream();
}


void File::closeCaptureStream() {
    _file->closeCaptureStream();
}


bool File::read(Buffer &buffer) {
    return _file->read(buffer);
}


bool File::write(Buffer buffer) {
    return _file->write(buffer);
}


bool File::readHeader() {
    return _file->readHeader();
}


bool File::writeHeader() {
    return _file->writeHeader();
}


bool File::isAvailable() const {
    return true;
}


bool File::isExist(const std::string &path) {
    std::ifstream file(path);
    return file.good();
}


bool File::parsePath(std::string &path) {
    // formatting
    // TODO: do replacements
    std::replace( path.begin(), path.end(), '\\', '/');

    // splitting
    std::string::size_type extensionIndex = path.rfind('.');
    std::string::size_type destinationIndex = path.rfind('/');

    if (extensionIndex==std::string::npos) {
        // TODO: logger message no extension
        return false;
    }

    if (destinationIndex!=std::string::npos) {
        _destination = path.substr(0, destinationIndex);
    } else {
        destinationIndex = 0;
    }

    _name = path.substr(destinationIndex, extensionIndex);
    _extension = path.substr(extensionIndex);

    return true;
}


void File::generateName() {
    std::string fileName = _name;
    uint32_t i = 0;

    while (isExist(fileName+_extension)) {
        fileName = _name+'('+std::to_string(i)+')';
        i++;
    }
    _name = fileName;
}


bool File::finalize() {
    return _file->finalize();
}


WaveProperties File::getStreamWaveProperties() const{
    return _file->getStreamWaveProperties();
}


bool File::isInUsage() const {
    return _file->isInUsage();
}


std::string File::getName() const {
    return _name;
}

} // namespace CASM