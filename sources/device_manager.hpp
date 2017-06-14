// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_DEVICEMANAGER_HPP
#define CROSSAUDIOSTREAMMANAGER_DEVICEMANAGER_HPP

//WIN MM
#include "device_manager_windows_wasapi.hpp"
typedef DeviceManagerWindowsMMD DeviceManagerAbstract;
//

class DeviceManager {
public:
    DeviceManager() {
        deviceManager = new DeviceManagerAbstract;
    }
private:
    DeviceManagerBase* deviceManager;
};

#endif //CROSSAUDIOSTREAMMANAGER_DEVICEMANAGER_HPP
