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
    File(std::string filePath, bool isForceWriting = false);
    ~File() override;

    // FileInterface
    Buffer openCaptureStream(std::chrono::duration< double > bufferDuration) final;
    bool openRenderStream(Buffer buffer) final;
    void closeRenderStream() final;
    void closeCaptureStream() final;
    bool read(Buffer &buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() const final;
    bool isInUsage() const final;
    std::string getName() const final;
    WaveProperties getStreamWaveProperties() const final;

private:
    bool readHeader() final;
    bool writeHeader() final;
    bool finalize() final;
    bool isExist(const std::string &filePath);
    bool parsePath();
    bool generateName();

    std::string _path = "";
    std::string _name = "";
    std::string _extension = "";

    std::shared_ptr< FileInterface > _file;
};

} // namespace CASM

#endif //CASM_FILE_HPP
