/**
    @file device.hpp
    @copyright LGPLv3
**/

#ifndef CASM_DEVICE_HPP
#define CASM_DEVICE_HPP

#include <CASM/device_manager/device_base.hpp>
#include <CASM/core/end_point.hpp>


namespace CASM {

/// @class Device
/// @brief wrapper under Device fabric
/// @details not copyable (copy will contain same instance)
class Device final : public DeviceInterface {
public:
    Device();
    Device(void *deviceHandler, CASM::DeviceType deviceType);
    ~Device() override;

    /// EndPointInterface interface
    Buffer openCaptureStream(std::chrono::duration< double > duration) final;
    bool openRenderStream(Buffer buffer) final;
    void closeCaptureStream() final;
    void closeRenderStream() final;
    bool read(Buffer &buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() const final;
    bool isInUsage() const final;

    WaveProperties getDeviceWaveProperties() final;
    WaveProperties getStreamWaveProperties() const final;
    std::wstring getDescription() final;

private:
    std::shared_ptr< DeviceInterface > device;
};

}

#endif //CASM_DEVICE_HPP
