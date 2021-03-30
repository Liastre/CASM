/// @file device_manager.hpp
/// @brief class DeviceManager stores collection of devices

#pragma once

#include <CASM/device_manager/device_manager_base.hpp>


namespace CASM {

class DeviceManager {
public:
    DeviceManager();
    ~DeviceManager();

    int update();
    std::size_t getDeviceCount();
    Device& getDevice(std::size_t index);

private:
    std::shared_ptr<DeviceManagerBase> deviceManager;
};

} // namespace CASM
