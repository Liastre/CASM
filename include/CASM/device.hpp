// =============== DESCRIPTION ===============
// Class Device represent end point device
// not copyable (copy will contain same
// instance)
// ===========================================

#ifndef CASM_DEVICE_HPP
#define CASM_DEVICE_HPP

#include "CASM/device_manager/device_base.hpp"
#include <CASM/core/end_point.hpp>

/// @class Device
/// @brief wrapper under Device fabric
class Device final : public DeviceInterface {
public:
    Device();
    Device(void* deviceHandler, CASM::DeviceType deviceType);
    ~Device() override;

    /// EndPointInterface interface
    void init(std::chrono::duration<double> bufferDuration) final;
    void open(CASM::Access access) final;
    void close() final;
    bool read(Buffer& buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() final;

    WaveProperties getDeviceWaveProperties() final;
    std::wstring getDescription() final;

private:
    std::shared_ptr<DeviceInterface> device;
};

#endif //CASM_DEVICE_HPP
