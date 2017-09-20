// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#ifndef CASM_DEVICE_MANAGER_WINDOWS_WASAPI_HPP
#define CASM_DEVICE_MANAGER_WINDOWS_WASAPI_HPP

#include <CASM/device_manager/device_manager_base.hpp>


class DeviceManagerWindowsWASAPI : public DeviceManagerBase {
public:
    DeviceManagerWindowsWASAPI();
    ~DeviceManagerWindowsWASAPI() final;
    int update() final;
};

#endif //CASM_DEVICE_MANAGER_WINDOWS_WASAPI_HPP
