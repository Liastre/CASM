// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#ifndef CASM_DEVICE_MANAGER_WINDOWS_WASAPI_HPP
#define CASM_DEVICE_MANAGER_WINDOWS_WASAPI_HPP

#include "windows_wasapi.hpp"
#include "device_manager_base.hpp"
#include "device_windows_wasapi.hpp"


class DeviceManagerWindowsWASAPI : public DeviceManagerBase<DeviceWindowsWASAPI> {
public:
    DeviceManagerWindowsWASAPI();
    ~DeviceManagerWindowsWASAPI();
    int update();
};

#endif //CASM_DEVICE_MANAGER_WINDOWS_WASAPI_HPP
