// =============== DESCRIPTION ===============
// Class Device represent end point device
// not copyable (copy will contain same
// instance)
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_DEVICE_HPP
#define CROSSAUDIOSTREAMMANAGER_DEVICE_HPP

#include "CASM/device_manager/device_base.hpp"


class Device {
public:
    Device();
    Device(void* deviceHandler, CASM::DeviceType deviceType);
    ~Device();

    int open(CASM::Access access, std::chrono::duration<double> fragmentDuration);
    int close();
    Buffer read();
    int write(Buffer data);

    WaveProperties getBufferWaveProperties();
    std::wstring getDescpiption();

private:
    std::shared_ptr<DeviceBase> device;
};

#endif //CROSSAUDIOSTREAMMANAGER_DEVICE_HPP
