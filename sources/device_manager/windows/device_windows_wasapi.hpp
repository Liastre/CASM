// =============== DESCRIPTION ===============
// ===========================================

#ifndef CASM_DEVICE_WINDOWS_WASAPI_HPP
#define CASM_DEVICE_WINDOWS_WASAPI_HPP

#include "windows_wasapi.hpp"
#include "CASM/device_manager/device_base.hpp"
#include <cassert>

typedef IMMDevice DeviceHandler;

class DeviceWindowsWASAPI : public DeviceTemplate<IMMDevice> {
public:
    DeviceWindowsWASAPI();
    DeviceWindowsWASAPI(void* device, CASM::DeviceType deviceType);
    ~DeviceWindowsWASAPI();
    int open(CASM::Access, std::chrono::duration<double> fragmentDuration);
    int close();
    Buffer read();
    int write(Buffer data);

private:
    IAudioClient* stream;
    IAudioCaptureClient *captureClient;
    IAudioRenderClient *renderClient;
};

#endif //CASM_DEVICE_WINDOWS_WASAPI_HPP
