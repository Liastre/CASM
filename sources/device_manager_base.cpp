#include "device_manager_base.hpp"

DeviceManagerBase::DeviceManagerBase() {
};

DeviceManagerBase::~DeviceManagerBase() {
};

Device DeviceManagerBase::getDevice(uint_fast32_t index) {
    if (index<deviceCount) {
        return deviceList[index];
    }
    return Device();
};

uint_fast32_t DeviceManagerBase::getDeviceCount() {
    return deviceCount;
}
