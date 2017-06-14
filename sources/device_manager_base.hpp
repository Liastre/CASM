// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_BASEDEVICEMANAGER_HPP
#define CROSSAUDIOSTREAMMANAGER_BASEDEVICEMANAGER_HPP

#include "wave_properties.hpp"
#include "device.hpp"
#include <CASM/CASM.hpp>
#include <vector>
#include <string>


class DeviceManagerBase {
public:
    DeviceManagerBase();
    virtual ~DeviceManagerBase();

    Device getDevice(uint_fast32_t index);
    uint_fast32_t getDeviceCount();

    virtual int update() = 0;

protected:
    std::vector<Device> deviceList;
    uint_fast32_t deviceCount;
};

#endif //CROSSAUDIOSTREAMMANAGER_BASEDEVICEMANAGER_HPP
