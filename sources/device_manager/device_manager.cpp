#include "CASM/device_manager.hpp"
#include "windows/device_manager_windows_wasapi.hpp"

namespace CASM {

DeviceManager::DeviceManager() {
    deviceManager = std::make_unique<DeviceManagerWindowsWASAPI>();
}

DeviceManager::~DeviceManager() {
    deviceManager.reset();
}

int
DeviceManager::update() {
    return deviceManager->update();
}

std::size_t
DeviceManager::getDeviceCount() {
    return deviceManager->getDeviceCount();
}

Device&
DeviceManager::getDevice(std::size_t index) {
    return deviceManager->getDevice(index);
}

} // namespace CASM
