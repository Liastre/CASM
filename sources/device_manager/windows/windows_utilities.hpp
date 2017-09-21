/// @file windows_utilities.hpp
/// @brief class for basic Windows methods

#ifndef CASM_ERROR_HANDLER_HPP
#define CASM_ERROR_HANDLER_HPP

#include <sstream>


namespace CASM {

class WinUtils {
public:
    static std::string HRESULTtoString(int32_t hr);
};

}

#endif //CASM_ERROR_HANDLER_HPP
