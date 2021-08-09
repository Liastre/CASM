#include "CASM/utility/windows_utilities.hpp"
#include <windows.h>
#include <sstream>

namespace CASM {

namespace Util {

std::string
String::wideToUtf8(wchar_t* wideString) {
    std::size_t stringSize = ::WideCharToMultiByte(CP_UTF8, NULL, wideString, -1, NULL, 0, 0, 0);
    std::string utf8String(stringSize, '\0');
    ::WideCharToMultiByte(CP_UTF8, NULL, wideString, -1, &utf8String[0], stringSize, 0, 0);

    return utf8String;
}

std::wstring
String::utf8ToWide(char const* narrowString) {
    std::size_t stringSize = ::MultiByteToWideChar(CP_UTF8, NULL, narrowString, -1, 0, 0);
    std::wstring wideString(stringSize, '\0');
    ::MultiByteToWideChar(CP_UTF8, NULL, narrowString, -1, &wideString[0], stringSize);
    return wideString;
}

std::wstring
String::utf8ToWide(std::string const& narrowString) {
    return utf8ToWide(narrowString.c_str());
}


} // namespace Util

std::string
WinUtils::HRESULTtoString(int32_t hr) {
    std::stringstream ss;
    ss << std::hex << (uint32_t)hr << std::endl;
    return ss.str();
}

} // namespace CASM
