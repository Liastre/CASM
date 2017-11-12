/// @file CASM.hpp
/// @brief main header of library

#ifndef CASM_CASM_HPP
#define CASM_CASM_HPP

#include "CASMconfig.hpp"
#include <cstdint>
#include <array>


namespace CASM {

// TODO: handle exclusive types
enum Access {
    READ, READ_EXCLUSIVE, WRITE, WRITE_EXCLUSIVE
};

enum DeviceType {
    CAPTURE, RENDER
};

} // end of namespace CASM

#endif //CASM_CASM_HPP
