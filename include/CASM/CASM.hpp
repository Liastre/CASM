/// @file CASM.hpp
/// @brief main header of library

#ifndef CASM_CASM_HPP
#define CASM_CASM_HPP

#include "CASMconfig.hpp"
#include <cstdint>
#include <array>
#include <chrono>


namespace CASM {

// TODO: handle exclusive types
enum class Access {
    READ,
    READ_EXCLUSIVE,
    WRITE,
    WRITE_EXCLUSIVE,
};

enum class DeviceType {
    CAPTURE,
    RENDER,
};

// Base typedefs
typedef std::chrono::duration<double> Duration;

} // end of namespace CASM

#endif //CASM_CASM_HPP
