/// @file windows_utilities.cpp
/// @brief definition of Windows utilities

#include "windows_utilities.hpp"

namespace CASM {

std::string WinUtils::HRESULTtoString(int32_t hr) {
    std::stringstream ss;
    ss << std::hex << (uint32_t) hr << std::endl;
    return ss.str();
}

}