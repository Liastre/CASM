/**
    @file device.hpp
    @copyright LGPLv3
**/

#pragma once

#include <CASM/device_manager/device_base.hpp>
#include <CASM/core/end_point.hpp>

namespace CASM {

/// @class Device
/// @brief wrapper under Device fabric
/// @details not copyable (copy will contain same instance)
class Device final : public DeviceInterface {
public:
    Device();
    Device(void* deviceHandler, DeviceType deviceType);
    Device(Device const& device);
    Device(Device&& device) noexcept;
    Device& operator=(Device const& device);
    Device& operator=(Device&& device);
    ~Device() override;

    /// EndPointInterface interface
    bool openCaptureStream(Duration const& duration, Buffer& buffer) final;
    bool openRenderStream(Buffer const& buffer) final;
    void closeCaptureStream() final;
    void closeRenderStream() final;
    BufferStatus read(Buffer& buffer) final;
    bool write(Buffer const& buffer) final;

    // TODO: add getState method
    bool isAvailable() const final;
    bool isInUsage() const final;
    bool isValid() const final;

    WaveProperties getDeviceWaveProperties() final;
    WaveProperties getStreamWaveProperties() const final;
    std::wstring getDescription() final;

    // operators
    operator bool() const;

private:
    std::shared_ptr<DeviceInterface> _device;
};

} // namespace CASM
