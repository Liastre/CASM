#ifndef CASM_FILE_BASE_HPP
#define CASM_FILE_BASE_HPP

#include <CASM/core/end_point.hpp>


namespace CASM {

namespace little_endian {

template < typename T >
void write(std::fstream &stream, const T &data, uint32_t dataSize = 0) {
    if (dataSize==0) {
        stream.write((const char *) &data, sizeof(T));
    } else {
        stream.write((const char *) &data, dataSize);
    }
}


template < typename T >
void read(std::fstream &stream, const T &data, uint32_t dataSize = 0) {
    if (dataSize==0) {
        stream.read(reinterpret_cast<char *>(const_cast<T *>(&data)), sizeof(T));
    } else {
        stream.read(reinterpret_cast<char *>(const_cast<T *>(&data)), dataSize);
    }
}

}

// TODO: remake, switch bits?
namespace big_endian {

template < typename Word >
std::ostream &write(std::ostream &outs, Word value, unsigned size = sizeof(Word)) {
    for (; size; --size, value >>= 8)
        outs.put(static_cast <char> (value & 0xFF));
    return outs;
}

}

class FileInterface : public virtual EndPointInterface {
public:
    FileInterface() = default;
    ~FileInterface() override = default;

    virtual bool readHeader()=0;
    virtual bool writeHeader()=0;
    virtual bool finalize()=0;
    virtual void setPath(std::string const & path)=0;
};


class FileBase : public virtual FileInterface, public EndPointBase {
public:
    FileBase() = default;
    FileBase(std::string const & filePath);
    ~FileBase() override = default;

    bool openCaptureStream(Duration const & bufferDuration, Buffer & buffer) final;
    bool openRenderStream(Buffer const & buffer) final;
    void closeCaptureStream() final;
    void closeRenderStream() final;
    bool isAvailable() const final;
    void setPath(std::string const & path) final;

protected:
    // TODO: fix memory leak
    std::fstream *_stream = new std::fstream();
    std::string _path = "";
};

}

#endif //CASM_FILE_BASE_HPP
