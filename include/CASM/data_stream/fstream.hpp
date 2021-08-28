#include "data_stream.hpp"
#include <fstream>

namespace CASM {

namespace DataStream {

class Fstream final : public DataStreamInterface {
public:
    Fstream();
    bool open(Access access, std::string const& dataPath) final;
    bool close() final;
    bool write(char const* data, std::size_t dataSize) final;
    bool read(char* data, std::size_t dataSize) final;
    std::size_t tellPos() final;
    bool seekPos(std::size_t pos) final;
    bool isEof() const final;
    bool isGood() const final;

private:
    Access _access;
    std::fstream _stream;
    std::string _path;
};

} // namespace FileHandler

} // namespace CASM
