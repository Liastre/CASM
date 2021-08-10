#include "CASM/utility/windows_utilities.hpp"
#include <windows.h>
#include <sstream>
#include <fstream>

namespace CASM {

namespace Util {

std::string
String::wideToUtf8(wchar_t* wideString) {
    // TODO: handle wrong size
    std::size_t stringSize = ::WideCharToMultiByte(CP_UTF8, NULL, wideString, -1, NULL, 0, 0, 0) - 1;
    std::string utf8String(stringSize, '\0');
    ::WideCharToMultiByte(CP_UTF8, NULL, wideString, -1, &utf8String[0], stringSize, 0, 0);
    utf8String.shrink_to_fit();

    return utf8String;
}

std::wstring
String::utf8ToWide(char const* narrowString) {
    // TODO: handle wrong size
    std::size_t stringSize = ::MultiByteToWideChar(CP_UTF8, NULL, narrowString, -1, 0, 0) - 1;
    std::wstring wideString(stringSize, '\0');
    ::MultiByteToWideChar(CP_UTF8, NULL, narrowString, -1, &wideString[0], stringSize);
    return wideString;
}

std::wstring
String::utf8ToWide(char const* narrowString, std::size_t size) {
    std::wstring wideString(size, '\0');
    ::MultiByteToWideChar(CP_UTF8, NULL, narrowString, -1, &wideString[0], size);
    wideString.erase(wideString.find(L'\0'));
    return wideString;
}

std::wstring
String::utf8ToWide(std::string const& narrowString) {
    return utf8ToWide(narrowString.c_str(), narrowString.size());
}

bool
Filesystem::isExist(char const* narrowPath) {
    std::ifstream file(String::utf8ToWide(narrowPath));
    return file.good();
}

bool
Filesystem::isExist(std::string const& narrowPath) {
    return isExist(narrowPath.c_str());
}

} // namespace Util

std::string
WinUtils::HRESULTtoString(int32_t hr) {
    std::stringstream ss;
    ss << std::hex << (uint32_t)hr << std::endl;
    return ss.str();
}

} // namespace CASM
