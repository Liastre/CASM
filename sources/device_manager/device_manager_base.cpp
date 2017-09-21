/// @file device_manager_base.cpp
/// @brief definition of DeviceManagerBase class

#include <CASM/device_manager/device_manager_base.hpp>


namespace CASM {

DeviceManagerBase::DeviceManagerBase() {
    deviceCount = 0;
};


DeviceManagerBase::~DeviceManagerBase() {
};


Device DeviceManagerBase::getDevice(uint_fast32_t index) {
    if (index < deviceCount) {
        return deviceList[index];
    }
    throw std::logic_error("Device index requested out of range");
};


uint_fast32_t DeviceManagerBase::getDeviceCount() {
    return deviceCount;
}

}
