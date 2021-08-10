/**
 * DeviceManager WASAPI implementation
 */

#pragma once

#include <CASM/device_manager/device_manager_base.hpp>

namespace CASM {

class DeviceManagerWindowsWASAPI : public DeviceManagerBase {
public:
    DeviceManagerWindowsWASAPI();
    ~DeviceManagerWindowsWASAPI() final;
    int update() final;
};

} // namespace CASM
