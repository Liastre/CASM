#include "CASM/data_stream/fstream.hpp"
#include "CASM/utility/windows_utilities.hpp"

namespace CASM {

namespace DataStream {

Fstream::Fstream(std::string const& filePath) {
    _path = filePath;
}

bool
Fstream::open(Access access) {
    _access = access;
    auto path = Util::String::utf8ToWide(_path);
    switch (access) {
    case Access::WRITE:
        _stream.open(path, std::ios::out | std::ios::binary);
        break;
    case Access::READ:
        _stream.open(path, std::ios::in | std::ios::binary);
        break;
    default:
        return false;
    }

    return true;
}

bool
Fstream::close() {
    _stream.close();
    return !_stream.good();
}

bool
Fstream::write(char const* data, std::size_t dataSize) {
    _stream.write(data, dataSize);
    return true;
}

bool
Fstream::read(char* data, std::size_t dataSize) {
    _stream.read(data, dataSize);
    return true;
}

std::size_t
Fstream::tellPos() {
    switch (_access) {
    case Access::WRITE:
        return static_cast<std::size_t>(_stream.tellp());
        break;
    case Access::READ:
        return static_cast<std::size_t>(_stream.tellg());
        break;
    default:
        break;
    }
}

bool
Fstream::seekPos(std::size_t pos) {
    switch (_access) {
    case Access::WRITE:
        _stream.seekp(pos);
        break;
    case Access::READ:
        _stream.seekg(pos);
        break;
    default:
        break;
    }

    return true;
}

bool
Fstream::isEof() const {
    return _stream.eof();
}

bool
Fstream::isGood() const {
    return _stream.good();
}

} // namespace DataStream

} // namespace CASM
