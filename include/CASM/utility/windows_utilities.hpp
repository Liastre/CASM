/// @file windows_utilities.hpp
/// @brief class for basic Windows methods

#pragma once

#include <string>

namespace CASM {

namespace Util {

namespace String {

std::string wideToUtf8(wchar_t* lpwstr);
std::wstring utf8ToWide(char const* narrowString);
std::wstring utf8ToWide(char const* narrowString, std::size_t size);
std::wstring utf8ToWide(std::string const& narrowString);

} // namespace String

} // namespace Util

class WinUtils {
public:
    static std::string HRESULTtoString(int32_t hr);
};

} // namespace CASM
