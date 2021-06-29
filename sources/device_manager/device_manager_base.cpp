/// @file device_manager_base.cpp
/// @brief definition of DeviceManagerBase class

#include <CASM/device_manager/device_manager_base.hpp>


namespace CASM {

DeviceManagerBase::DeviceManagerBase() {
    _deviceCount = 0;
};


DeviceManagerBase::~DeviceManagerBase() {
};


Device& DeviceManagerBase::getDevice(std::size_t index) {
    return _deviceList[index];
};


std::size_t DeviceManagerBase::getDeviceCount() {
    return _deviceCount;
}

}
