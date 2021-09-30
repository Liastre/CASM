/**
 * Supplimetary library types
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include <cstdint>
#include <chrono>
#include <string>

namespace CASM {

// TODO: handle exclusive types
enum class Access {
    Undefined,
    READ,
    READ_EXCLUSIVE,
    WRITE,
    WRITE_EXCLUSIVE,
};

enum class DeviceType {
    Undefined,
    CAPTURE,
    RENDER,
};

// Base typedefs
using Duration = std::chrono::duration<double>;
using String = std::string;
using FramesCount = std::uint32_t;

} // end of namespace CASM
