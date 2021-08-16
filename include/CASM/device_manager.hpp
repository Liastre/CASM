/**
 * @brief DeviceManager for recieving and storing
 * collection of system devices
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include "device_manager/device_manager_base.hpp"

namespace CASM {

class DeviceManager final {
public:
    template <typename T>
    DeviceManager(T&& deviceEnumerator) {
        static_assert(std::is_base_of<DeviceApi::EnumeratorBase, T>::value,
          "Passed DeviceManager is not derived from DeviceManagerBase");

        _deviceEnumerator = std::make_unique<T>(std::move(deviceEnumerator));
        _deviceEnumerator->update();
    }
    ~DeviceManager();

    int update();
    std::size_t getDeviceCount();
    Device& getDevice(std::size_t index);

private:
    std::unique_ptr<DeviceApi::EnumeratorBase> _deviceEnumerator;
};

} // namespace CASM
