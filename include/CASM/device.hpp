/// @file Device.hpp
/// @brief class Device
/**
    Description:
    Class Device represent end point device
    Details:
    Not copyable (copy will contain same instance)
**/

#ifndef CASM_DEVICE_HPP
#define CASM_DEVICE_HPP

#include <CASM/device_manager/device_base.hpp>
#include <CASM/core/end_point.hpp>


namespace CASM {

/// @class Device
/// @brief wrapper under Device fabric
class Device final : public DeviceInterface {
public:
    Device();
    Device(void *deviceHandler, CASM::DeviceType deviceType);
    ~Device() override;

    /// EndPointInterface interface
    Buffer open(std::chrono::duration< double > duration) override;

    bool open(Buffer buffer) final;
    void close() final;
    bool read(Buffer &buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() final;

    WaveProperties getDeviceWaveProperties() final;
    WaveProperties getStreamWaveProperties() final;
    std::wstring getDescription() final;

private:
    std::shared_ptr< DeviceInterface > device;
};

}

#endif //CASM_DEVICE_HPP
