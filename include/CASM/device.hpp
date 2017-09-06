// =============== DESCRIPTION ===============
// Class Device represent end point device
// not copyable (copy will contain same
// instance)
// ===========================================

#ifndef CASM_DEVICE_HPP
#define CASM_DEVICE_HPP

#include "CASM/device_manager/device_base.hpp"
#include <CASM/core/end_point.hpp>


class Device final : public EndPoint {
public:
    Device();
    Device(void* deviceHandler, CASM::DeviceType deviceType);
    ~Device();

    Buffer read() final;
    void write(Buffer data) final;
    bool isAvailable() final;

    /// open - open device and initialize the buffer
    /// @param [in] access - requested access to device
    /// @param [in] bufferDuration - size of device buffer (since you need wait until buffer are filled, output delay will be the same)
    int open(CASM::Access access, std::chrono::duration<double> bufferDuration);
    int close();

    WaveProperties getStreamWaveProperties();
    std::wstring getDescpiption();

private:
    std::shared_ptr<DeviceBase> device;
};

#endif //CASM_DEVICE_HPP
