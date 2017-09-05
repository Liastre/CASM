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
    int open(CASM::Access, std::chrono::duration<double> fragmentDuration) final;
    int close() override;
    Buffer read() override;
    int write(Buffer data) override;

private:
    IAudioClient* stream;
    HANDLE hEvent;
    IAudioCaptureClient *captureClient;
    IAudioRenderClient *renderClient;
};

#endif //CASM_DEVICE_WINDOWS_WASAPI_HPP
