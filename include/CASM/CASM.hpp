/**
 * Main header of library
 * @author Liastre
 * @copyright MIT
 */

#pragma once

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
