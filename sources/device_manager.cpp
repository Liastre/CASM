#include "CASM/device_manager.hpp"

namespace CASM {

DeviceManager::~DeviceManager() {
    _deviceEnumerator.reset();
}

bool
DeviceManager::update() {
    bool isSuccess = _deviceEnumerator->update(_deviceList);
    _deviceCount = _deviceList.size();
    return isSuccess;
}

std::size_t
DeviceManager::getDeviceCount() {
    return _deviceCount;
}

Device&
DeviceManager::getDevice(std::size_t index) {
    return _deviceList[index];
}

} // namespace CASM
