/**
 * @brief DeviceManager for recieving and storing
 * collection of system devices
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include "device_manager/device_manager_base.hpp"

namespace CASM {

// TODO: rework for using different APIs
class DeviceManager final {
public:
    DeviceManager();
    ~DeviceManager();

    int update();
    std::size_t getDeviceCount();
    Device& getDevice(std::size_t index);

private:
    std::unique_ptr<DeviceManagerBase> deviceManager;
};

} // namespace CASM
