// =============== DESCRIPTION ===============
// ===========================================

#ifndef CASM_DEVICE_WINDOWS_WASAPI_HPP
#define CASM_DEVICE_WINDOWS_WASAPI_HPP

#include "windows_wasapi.hpp"
#include "device_base.hpp"
#include <cassert>


class DeviceWindowsWASAPI : public DeviceTemplate<IMMDevice, IAudioClient*> {
public:
    DeviceWindowsWASAPI();
    DeviceWindowsWASAPI(DeviceHandler *device, CASM::DeviceType deviceType);
    ~DeviceWindowsWASAPI();
    int open(CASM::Access, std::chrono::duration<double> fragmentDuration);
    int close();
    Buffer read();
    int write(Buffer data);

private:
    IAudioCaptureClient *captureClient;
    IAudioRenderClient *renderClient;
};

#endif //CASM_DEVICE_WINDOWS_WASAPI_HPP
