// =============== DESCRIPTION ===============
// Class Device represent end point device
// not copyable (copy will contain same
// instance)
// ===========================================

#ifndef CASM_DEVICE_HPP
#define CASM_DEVICE_HPP

#include "CASM/device_manager/device_base.hpp"


class Device {
public:
    Device();
    Device(void* deviceHandler, CASM::DeviceType deviceType);
    ~Device();

    /// open - open device and initialize the buffer
    /// @param [in] access - requested access to device
    /// @param [in] bufferDuration - size of device buffer (since you need wait until buffer are filled, output delay will be the same)
    int open(CASM::Access access, std::chrono::duration<double> bufferDuration);
    int close();
    Buffer read();
    int write(Buffer data);

    WaveProperties getStreamWaveProperties();
    std::wstring getDescpiption();

private:
    std::shared_ptr<DeviceBase> device;
};

#endif //CASM_DEVICE_HPP
