// =============== DESCRIPTION ===============
// ===========================================

#ifndef CASM_DEVICE_WINDOWS_WASAPI_HPP
#define CASM_DEVICE_WINDOWS_WASAPI_HPP

#include "windows_wasapi.hpp"
#include "CASM/device_manager/device_base.hpp"
#include <cassert>

typedef IMMDevice DeviceHandler;

class DeviceWindowsWASAPI final : public DeviceTemplate<IMMDevice> {
public:
    DeviceWindowsWASAPI();
    DeviceWindowsWASAPI(void* device, CASM::DeviceType deviceType);
    ~DeviceWindowsWASAPI() final;

    /// EndPointInterface
    void open(CASM::Access) final;
    void close() final;
    bool read(Buffer& buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() final;

private:
    IAudioClient* stream;
    HANDLE hEvent;
    IAudioCaptureClient *captureClient;
    IAudioRenderClient *renderClient;
};

#endif //CASM_DEVICE_WINDOWS_WASAPI_HPP
