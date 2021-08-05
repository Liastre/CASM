/**
    @copyright LGPLv3
**/

#pragma once

#include <CASM/CASM.hpp>
#include <CASM/core/file_base.hpp>
#include <CASM/core/buffer.hpp>

#include <string>
#include <fstream>

namespace CASM {

class FileInterface {
public:
    virtual ~FileInterface() = default;

    virtual bool open(Access access) = 0;
    virtual bool close() = 0;
    virtual WaveProperties readHeader() = 0;
    virtual bool writeHeader(WaveProperties const& waveProperties) = 0;
    virtual BufferStatus readData(Buffer& buffer) = 0;
    virtual bool writeData(Buffer const& buffer) = 0;
    virtual bool finalize() = 0;
    virtual bool isGood() const = 0;
};

/**
 * File wrapper, wraps existing or
 * user defined File type class
 */
template <class TFileType>
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
    std::shared_ptr<FileInterface> _file;
    // TODO: replace with library
    char kSplitPathSymbol = '/';
};

template <class TFileType>
File<TFileType>::File(std::string const& path, bool doCreateNewFileOnWrite) {
    _doCreateNewFileOnWrite = doCreateNewFileOnWrite;
    std::string resPath(path);
    _formatPath(resPath);
    if (!_parsePath(resPath)) {
        // log invalid path
    }
    _isValid = { true };
};

template <class TFileType>
File<TFileType>::~File() {
    _file.reset();
}

template <class TFileType>
bool // TODO: redo with result
File<TFileType>::openCaptureStream(Duration const& bufferDuration, Buffer& buffer) {
    if (!_isExist()) {
        return false;
    }

    _file = std::make_shared<TFileType>(_path);
    _file->open(Access::READ);
    _streamWaveProperties = _file->readHeader();
    buffer = Buffer(_streamWaveProperties, bufferDuration);
    // TODO: add readHeader check
    return true;
}

template <class TFileType>
bool
File<TFileType>::openRenderStream(Buffer const& buffer) {
    if (_isExist() && _doCreateNewFileOnWrite) {
        _generateName();
    }

    _file = std::make_shared<TFileType>(_path);
    if (!_file->open(Access::WRITE)) {
        return false;
    }

    _streamWaveProperties = buffer.getWaveProperties();
    return _file->writeHeader(_streamWaveProperties);
}

template <class TFileType>
void
File<TFileType>::closeRenderStream() {
    _file->finalize();
    _file->close();
}

template <class TFileType>
void
File<TFileType>::closeCaptureStream() {
    _file->close();
}

template <class TFileType>
BufferStatus
File<TFileType>::read(Buffer& buffer) {
    return _file->readData(buffer);
}

template <class TFileType>
bool
File<TFileType>::write(Buffer const& buffer) {
    return _file->writeData(buffer);
}

template <class TFileType>
std::string
File<TFileType>::getName() const {
    return _name;
}

template <class TFileType>
bool
File<TFileType>::isAvailable() const {
    return _isExist();
}

template <class TFileType>
bool
File<TFileType>::_isExist() const {
    std::ifstream file(_path);
    return file.good();
}

template <class TFileType>
void
File<TFileType>::_formatPath(std::string& path) {
    std::replace(path.begin(), path.end(), '\\', kSplitPathSymbol);
}

template <class TFileType>
bool
File<TFileType>::_parsePath(std::string const& path) {
    // TODO: there might be no extension
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

template <class TFileType>
void
File<TFileType>::_generateName() {
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

template <class TFileType>
File<TFileType>::operator bool() const {
    return isValid();
}

template <class C>
struct IsFile : std::false_type { };

template <class C>
struct IsFile<File<C>> : std::true_type { };

} // namespace CASM
