/// @file device_windows_wasapi.hpp
/// @brief class DeviceWindowsWASAPI derived from DeviceBase

#pragma once

#include "windows_wasapi.hpp"
#include "CASM/device_manager/device_base.hpp"

namespace CASM {
namespace DeviceApi {
namespace Wasapi {

typedef IMMDevice DeviceHandler;

class DeviceWindowsWASAPI final : public DeviceBase<IMMDevice> {
public:
    DeviceWindowsWASAPI();
    DeviceWindowsWASAPI(void* device, CASM::DeviceType deviceType);
    ~DeviceWindowsWASAPI() final;

    /// EndPointInterface
    bool openCaptureStream(Duration const& duration, Buffer& buffer) final;
    bool openRenderStream(Buffer const& buffer) final;
    void closeCaptureStream() final;
    void closeRenderStream() final;
    BufferStatus read(Buffer& buffer) final;
    bool write(Buffer const& buffer) final;
    bool isAvailable() const final;

private:
    IAudioClient* _captureStream = nullptr;
    IAudioClient* _renderStream = nullptr;
    IAudioCaptureClient* _captureClient = nullptr;
    IAudioRenderClient* _renderClient = nullptr;
    HANDLE _hEvent = nullptr;
};

} // namespace Wasapi
} // namespace DeviceApi
} // namespace CASM
