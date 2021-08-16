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
using Duration = std::chrono::duration<double>;
using String = std::string;

} // end of namespace CASM
