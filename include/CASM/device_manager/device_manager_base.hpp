/**
 * Base class for DeviceManager
 */

#pragma once

#include <CASM/device.hpp>
#include "CASM/core/wave_properties.hpp"
#include <vector>
#include <string>

namespace CASM {

class DeviceManagerBase {
public:
    DeviceManagerBase();
    virtual ~DeviceManagerBase();

    Device& getDevice(std::size_t index);
    std::size_t getDeviceCount();

    virtual int update() = 0;

protected:
    std::vector<Device> _deviceList;
    std::size_t _deviceCount;
};

} // namespace CASM
