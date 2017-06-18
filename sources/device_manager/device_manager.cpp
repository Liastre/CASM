// =============== DESCRIPTION ===============
// ===========================================

#include <CASM/device_manager.hpp>
#include "windows/device_manager_windows_wasapi.hpp"

DeviceManager::DeviceManager() {
    deviceManager = std::make_shared<DeviceManagerWindowsWASAPI>();
}

int DeviceManager::update() {
    return deviceManager->update();
}

uint_fast32_t DeviceManager::getDeviceCount() {
    return deviceManager->getDeviceCount();
}

Device DeviceManager::getDevice(uint_fast32_t index) {
    return deviceManager->getDevice(index);
}

