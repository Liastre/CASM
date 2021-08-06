#pragma once

#include <CASM/CASM.hpp>

namespace CASM {

namespace DataStream {

class DataStreamInterface {
public:
    virtual ~DataStreamInterface() = default;

    virtual bool open(Access) = 0;
    virtual bool close() = 0;
    virtual bool write(char const* data, std::size_t dataSize) = 0;
    virtual bool read(char* data, std::size_t dataSize) = 0;
    virtual std::size_t tellPos() = 0;
    virtual bool seekPos(std::size_t pos) = 0;
    virtual bool isEof() const = 0;
    virtual bool isGood() const = 0;

    template <typename T>
    void
    read(T& data, std::size_t dataSize = 0) {
        if (dataSize == 0) {
            read(reinterpret_cast<char*>(&data), sizeof(T));
        } else {
            read(reinterpret_cast<char*>(&data), dataSize);
        }
    }

    template <typename T>
    void
    write(T const& data, std::size_t dataSize = 0) {
        if (dataSize == 0) {
            write(reinterpret_cast<char const*>(&data), sizeof(T));
        } else {
            write(reinterpret_cast<char const*>(&data), dataSize);
        }
    }
};

} // namespace DataStream

} // namespace CASM