/// @file device_manager_windows_wasapi.hpp
/// @brief class DeviceManagerWindowsWASAPI derived from DeviceManagerBase

#ifndef CASM_DEVICE_MANAGER_WINDOWS_WASAPI_HPP
#define CASM_DEVICE_MANAGER_WINDOWS_WASAPI_HPP

#include <CASM/device_manager/device_manager_base.hpp>


namespace CASM {

class DeviceManagerWindowsWASAPI : public DeviceManagerBase {
public:
    DeviceManagerWindowsWASAPI();
    ~DeviceManagerWindowsWASAPI() final;
    int update() final;
};

}

#endif //CASM_DEVICE_MANAGER_WINDOWS_WASAPI_HPP
