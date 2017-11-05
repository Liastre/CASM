/// @file file.cpp
/// @brief definition of File class

#include "CASM/file.hpp"
#include "file/file_wave.hpp"

#include <cstdint>
#include <cstring>


namespace CASM {

File::File(std::string filePath, bool isForceWriting) {
    _path = filePath;
    if(parsePath()) {
        if(!isForceWriting) {
            generateName();
        }
        _file = std::make_shared< FileWave >(_path);
    } else {
        // report no extension
    }
}


File::~File() {
    _file.reset();
}


Buffer File::openCaptureStream(std::chrono::duration< double > bufferDuration) {
    if (!isExist(_path)) {
        return (Buffer());
    }
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


bool File::isExist(const std::string &filePath) {
    std::ifstream f(filePath.c_str());
    return f.good();
}


bool File::parsePath() {
    std::string::size_type idx;
    idx = _path.rfind('.');

    if (idx==std::string::npos) {
        return false;
    }

    _extension = _path.substr(idx+1);
    _name = _path.substr(0, idx);

    return true;
}


bool File::generateName() {
    std::string fileName = _name;
    uint32_t i = 0;

    while (isExist(fileName+'.'+_extension)) {
        fileName = _name+'('+std::to_string(i)+')';
        i++;
    }
    _name = fileName;

    return true;
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
    return _file->getName();
}

} // namespace CASM