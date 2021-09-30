#include "CASM/utility/windows_utilities.hpp"
#include <windows.h>
#include <sstream>
#include <fstream>
#include <algorithm>

namespace {

constexpr char kUnixSplitPathSymbol = '/';
constexpr char kWinSplitPathSymbol = '\\';

} // namespace

namespace CASM {

namespace Util {

std::string
String::wideToUtf8(wchar_t* wideString) {
    // TODO: handle wrong size
    std::size_t stringSize = ::WideCharToMultiByte(CP_UTF8, NULL, wideString, -1, NULL, 0, 0, 0) - 1;
    std::string utf8String(stringSize, '\0');
    ::WideCharToMultiByte(CP_UTF8, NULL, wideString, -1, &utf8String[0], stringSize, 0, 0);

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
    auto termPos = wideString.find(L'\0');
    if (termPos != std::wstring::npos) {
        wideString.erase(termPos);
    }
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

std::string::size_type
Filesystem::findExtensionPos(std::string const& path) {
    std::string::size_type extensionIndex = path.rfind('.');
    std::string::size_type destinationIndex = findLastPartPos(path);
    // no extension found
    if (extensionIndex == std::string::npos) {
        return std::string::npos;
    }

    // dot is part of the path
    if (destinationIndex != std::string::npos && destinationIndex > extensionIndex) {
        return std::string::npos;
    }

    return extensionIndex;
}

std::string::size_type
Filesystem::findLastPartPos(std::string const& path) {
    std::string::size_type unixDelimerIndex = path.rfind(kUnixSplitPathSymbol);
    std::string::size_type winDelimerIndex = path.rfind(kWinSplitPathSymbol);
    if (unixDelimerIndex == std::string::npos && winDelimerIndex == std::string::npos) {
        return 0;
    }

    std::string::size_type delimerIndex{ 0 };
    if (unixDelimerIndex == std::string::npos) {
        delimerIndex = winDelimerIndex;
    } else if (winDelimerIndex == std::string::npos) {
        delimerIndex = unixDelimerIndex;
    } else {
        delimerIndex = (winDelimerIndex > unixDelimerIndex) ? winDelimerIndex : unixDelimerIndex;
    }

    return ++delimerIndex;
}

std::string
Filesystem::generateNextNameIfExist(std::string const& originalPath) {
    std::size_t i(0);
    std::string tmpName(originalPath);
    std::size_t extensionPos = findExtensionPos(originalPath);
    std::string extension;
    if (extensionPos != std::string::npos) {
        extension = originalPath.substr(extensionPos);
    }
    while (isExist(tmpName)) {
        tmpName.erase(extensionPos);
        tmpName.append("(")
            .append(std::to_string(i))
            .append(")")
            .append(extension);
        i++;
    }

    return tmpName;
}

void
Filesystem::formatPath(std::string& path) {
    std::replace(path.begin(), path.end(), '\\', kUnixSplitPathSymbol);
}

} // namespace Util

std::string
WinUtils::HRESULTtoString(int32_t hr) {
    std::stringstream ss;
    ss << std::hex << (uint32_t)hr << std::endl;
    return ss.str();
}

} // namespace CASM
