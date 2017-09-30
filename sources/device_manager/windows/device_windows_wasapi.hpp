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
    Buffer openCaptureStream(std::chrono::duration< double > duration) final;
    bool openRenderStream(Buffer buffer) final;
    void closeCaptureStream() final;
    void closeRenderStream() final;
    bool read(Buffer &buffer) final;
    bool write(Buffer buffer) final;
    bool isAvailable() final;

private:
    IAudioClient *captureStream;
    IAudioClient *renderStream;
    IAudioCaptureClient *captureClient;
    IAudioRenderClient *renderClient;
    HANDLE hEvent;
};

}

#endif //CASM_DEVICE_WINDOWS_WASAPI_HPP
