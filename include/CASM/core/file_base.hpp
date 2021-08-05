#pragma once

#include <CASM/file.hpp>
#include <CASM/core/end_point.hpp>

namespace CASM {

namespace little_endian {

template <typename T>
void
write(std::fstream& stream, const T& data, uint32_t dataSize = 0) {
    if (dataSize == 0) {
        stream.write((const char*)&data, sizeof(T));
    } else {
        stream.write((const char*)&data, dataSize);
    }
}

template <typename T>
void
read(std::fstream& stream, const T& data, uint32_t dataSize = 0) {
    if (dataSize == 0) {
        stream.read(reinterpret_cast<char*>(const_cast<T*>(&data)), sizeof(T));
    } else {
        stream.read(reinterpret_cast<char*>(const_cast<T*>(&data)), dataSize);
    }
}

} // namespace little_endian

// TODO: remake, switch bits?
namespace big_endian {

template <typename T>
std::ostream&
write(std::ostream& stream, T value, unsigned size = sizeof(T)) {
    for (; size; --size, value >>= 8)
        stream.put(static_cast<char>(value & 0xFF));
    return stream;
}

} // namespace big_endian

} // namespace CASM
