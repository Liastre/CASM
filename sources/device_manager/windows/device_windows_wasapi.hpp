/// @file device_windows_wasapi.hpp
/// @brief class DeviceWindowsWASAPI derived from DeviceBase

#ifndef CASM_DEVICE_WINDOWS_WASAPI_HPP
#define CASM_DEVICE_WINDOWS_WASAPI_HPP

#include "windows_wasapi.hpp"
#include "CASM/device_manager/device_base.hpp"
#include <cassert>


namespace CASM {

typedef IMMDevice DeviceHandler;

class DeviceWindowsWASAPI final : public DeviceBase< IMMDevice > {
public:
    DeviceWindowsWASAPI();
    DeviceWindowsWASAPI(void *device, CASM::DeviceType deviceType);
    ~DeviceWindowsWASAPI() final;

    /// EndPointInterface
    Buffer open(std::chrono::duration< double > duration) final;
    bool open(Buffer buffer) final;
    void close() final;
    bool read(Buffer &buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() final;

private:
    IAudioClient *stream;
    HANDLE hEvent;
    IAudioCaptureClient *captureClient;
    IAudioRenderClient *renderClient;
};

}

#endif //CASM_DEVICE_WINDOWS_WASAPI_HPP
