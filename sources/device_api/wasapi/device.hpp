/// @file device_windows_wasapi.hpp
/// @brief class DeviceWindowsWASAPI derived from DeviceBase

#pragma once

#include "windows_wasapi.hpp"
#include "CASM/device_api/device_base.hpp"

namespace CASM {
namespace DeviceApi {
namespace Wasapi {

class Device final : public DeviceBase {
public:
    using Handler = IMMDevice;

public:
    Device();
    Device(Handler* device, CASM::DeviceType deviceType);
    ~Device() final;

    StreamProperties openRead(Duration const& requestedDuration, bool isExclusive) final;
    StreamProperties openWrite(Duration const& requestedDuration, bool isExclusive) final;
    void closeRead() final;
    void closeWrite() final;
    BufferStatus readData(Buffer& buffer, std::uint16_t blockAlign) final;
    bool writeData(Buffer const& buffer, std::uint16_t blockAlign) final;
    String const& getDescription() const final;
    String const& getName() const final;

private:
    Handler* _handler = nullptr;
    IAudioClient* _captureStream = nullptr;
    IAudioClient* _renderStream = nullptr;
    IAudioCaptureClient* _captureClient = nullptr;
    IAudioRenderClient* _renderClient = nullptr;
    HANDLE _hEvent = nullptr;
    std::uint32_t _bufferFramesCount = 0;
    String _description;
    String _name;
};

} // namespace Wasapi
} // namespace DeviceApi
} // namespace CASM
