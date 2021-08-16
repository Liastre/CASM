#include "CASM/device_manager.hpp"

namespace CASM {

DeviceManager::~DeviceManager() {
    _deviceEnumerator.reset();
}

int
DeviceManager::update() {
    return _deviceEnumerator->update();
}

std::size_t
DeviceManager::getDeviceCount() {
    return _deviceEnumerator->getDeviceCount();
}

Device&
DeviceManager::getDevice(std::size_t index) {
    return _deviceEnumerator->getDevice(index);
}

} // namespace CASM
