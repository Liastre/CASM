// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_BASEDEVICEMANAGER_HPP
#define CROSSAUDIOSTREAMMANAGER_BASEDEVICEMANAGER_HPP

#include "wave_properties.hpp"
#include "device_base.hpp"
#include <CASM/CASM.hpp>
#include <vector>
#include <string>


template <class TDeviceBaseDerived>
class DeviceManagerBase {
public:
    DeviceManagerBase() {};
    virtual ~DeviceManagerBase() {};
    virtual int update() = 0;
    // TODO: copy constructor
    TDeviceBaseDerived getDevice(uint_fast32_t index) {
        if (index<deviceCount) {
            return deviceList[index];
        }
        return TDeviceBaseDerived();
    };
    uint_fast32_t getDeviceCount() {
        return deviceCount;
    }

protected:
    std::vector<TDeviceBaseDerived> deviceList;
    uint_fast32_t deviceCount;
};

#endif //CROSSAUDIOSTREAMMANAGER_BASEDEVICEMANAGER_HPP
