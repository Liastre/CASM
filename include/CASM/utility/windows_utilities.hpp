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

namespace Filesystem {

bool isExist(char const* narrowPath);
bool isExist(std::string const& narrowPath);
std::string::size_type findExtensionPos(std::string const& filepath);
std::string::size_type findLastPartPos(std::string const& path);
std::string generateNextNameIfExist(std::string const& originalName);
void formatPath(std::string& path);

} // namespace Filesystem

} // namespace Util

class WinUtils {
public:
    static std::string HRESULTtoString(int32_t hr);
};

} // namespace CASM
