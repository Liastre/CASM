// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#ifndef CASM_BASEDEVICEMANAGER_HPP
#define CASM_BASEDEVICEMANAGER_HPP

#include <CASM/device.hpp>
#include "CASM/core/wave_properties.hpp"
#include <vector>
#include <string>


namespace CASM {

class DeviceManagerBase {
public:
    DeviceManagerBase();
    virtual ~DeviceManagerBase();

    Device& getDevice(uint_fast32_t index);
    std::size_t getDeviceCount();

    virtual int update() = 0;

protected:
    std::vector<Device> _deviceList;
    std::size_t _deviceCount;
};

}

#endif //CASM_BASEDEVICEMANAGER_HPP
