/**
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include "device_api/device_base.hpp"

namespace CASM {

/**
 * Wrapper under Device fabric
 * not copyable (copy will contain same instance)
 */
class Device final : public DeviceApi::DeviceInterface {
public:
    Device();
    Device(void* deviceHandler, DeviceType deviceType);
    Device(Device const& device);
    Device(Device&& device) noexcept;
    Device& operator=(Device const& device);
    Device& operator=(Device&& device);
    ~Device();

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
    String getDescription() final;

    operator bool() const;

private:
    std::shared_ptr<DeviceInterface> _device;
};

} // namespace CASM
