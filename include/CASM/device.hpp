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
class Device final : public EndPointInterface {
public:
    template <typename T, std::enable_if_t<!std::is_same_v<Device, typename std::decay<T>::type>>* = nullptr>
    Device(T&& deviceApi) {
        static_assert(std::is_base_of<DeviceApi::DeviceInterface, T>::value,
          "Passed Device Api is not derived from DeviceInterface");

        _device = std::make_shared<T>(std::move(deviceApi));
    }
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

    WaveProperties getDeviceWaveProperties();
    WaveProperties getStreamWaveProperties() const;
    String getDescription();

    operator bool() const;

private:
    // TODO: use unique_ptr
    std::shared_ptr<DeviceApi::DeviceInterface> _device;
};

} // namespace CASM
