// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#ifndef CASM_DEVICEMANAGER_HPP
#define CASM_DEVICEMANAGER_HPP

#include <CASM/device_manager/device_manager_base.hpp>


namespace CASM {

class DeviceManager {
public:
    DeviceManager();
    int update();
    uint_fast32_t getDeviceCount();
    Device getDevice(uint_fast32_t index);

private:
    std::shared_ptr< DeviceManagerBase > deviceManager;
};

}

#endif //CASM_DEVICEMANAGER_HPP
