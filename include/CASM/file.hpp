/**
    @file file.hpp
    @copyright LGPLv3
**/

#ifndef CASM_FILE_HPP
#define CASM_FILE_HPP

#include <CASM/CASM.hpp>
#include <CASM/core/file_base.hpp>

#include <string>
#include <fstream>


namespace CASM {

enum FileType {
    WAV
};

class File final : public FileInterface {
public:
    File() = default;
    File(std::string const & path, bool shouldForceWriting = false);
    ~File() override;

    // FileInterface
    bool openCaptureStream(Duration const & bufferDuration, Buffer & buffer) final;
    bool openRenderStream(Buffer const & buffer) final;
    void closeRenderStream() final;
    void closeCaptureStream() final;
    BufferStatus read(Buffer & buffer) final;
    bool write(Buffer const & buffer) final;
    bool isAvailable() const final;
    bool isInUsage() const final;
    bool isValid() const final;
    std::string getName() const;
    WaveProperties getStreamWaveProperties() const final;

    // operators
    operator bool() const;

private:
    bool readHeader() final;
    bool writeHeader() final;
    bool finalize() final;
    void setPath(std::string const & path) final;
    bool _isExist() const;
    void _formatPath(std::string & path);
    bool _parsePath(std::string const & path);
    void _generateName();

    std::string _path = "";
    std::string _destination = "";
    std::string _name = "";
    std::string _extension = "";
    bool _shouldForceWriting = false;

    std::shared_ptr< FileInterface > _file;
};

} // namespace CASM

#endif //CASM_FILE_HPP
