#include "device.hpp"
#include "functiondiscoverykeys_devpkey.h"
#include <thread>
#include <mmreg.h>
#include <cassert>

namespace {

/*
// Temp methods, not used anywhere
// TODO: fix mingw build
enum class SubFormat {
    PCM,
    IEEE_FLOAT,
    DRM,
    ALAW,
    MULAW,
    ADPCM,
};

SubFormat
TranslateSubformatToEnum(GUID const& subFormat) {
    if (subFormat == KSDATAFORMAT_SUBTYPE_PCM)
        return SubFormat::PCM;
    if (subFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)
        return SubFormat::IEEE_FLOAT;
    if (subFormat == KSDATAFORMAT_SUBTYPE_DRM)
        return SubFormat::DRM;
    if (subFormat == KSDATAFORMAT_SUBTYPE_ALAW)
        return SubFormat::ALAW;
    if (subFormat == KSDATAFORMAT_SUBTYPE_MULAW)
        return SubFormat::MULAW;
    if (subFormat == KSDATAFORMAT_SUBTYPE_ADPCM)
        return SubFormat::ADPCM;
}*/

} // namespace

namespace CASM {
namespace DeviceApi {
namespace Wasapi {

Device::Device() {
    HRESULT hr = S_OK;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    assert(hr == S_OK);
}

Device::Device(Handler* device, CASM::DeviceType deviceType)
    : DeviceBase::DeviceBase(deviceType) {
    HRESULT hr = S_OK;
    LPWSTR deviceId = nullptr;
    IPropertyStore* propertyStore = nullptr;
    PROPVARIANT devicePROPVARIANT = {};
    _handler = device;

    // active
    PropVariantInit(&devicePROPVARIANT);

    // read device properties
    hr = _handler->OpenPropertyStore(STGM_READ, &propertyStore);
    if (hr != S_OK)
        throw std::runtime_error("Unable to handler->OpenPropertyStore(). Error code: " + WinUtils::HRESULTtoString(hr));

    // retrieve device name
    hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &devicePROPVARIANT);
    if (hr != S_OK) {
        throw std::runtime_error("Unable to propertyStore->GetValue(PKEY_Device_DeviceDesc). Error code: " + WinUtils::HRESULTtoString(hr));
    }
    _name = Util::String::wideToUtf8(devicePROPVARIANT.pwszVal);

    // retrieve device description
    hr = propertyStore->GetValue(PKEY_Device_DeviceDesc, &devicePROPVARIANT);
    if (hr != S_OK) {
        throw std::runtime_error("Unable to propertyStore->GetValue(PKEY_Device_FriendlyName). Error code: " + WinUtils::HRESULTtoString(hr));
    }
    _description = Util::String::wideToUtf8(devicePROPVARIANT.pwszVal);

    // device id
    hr = _handler->GetId(&deviceId);
    if (hr != S_OK)
        throw std::runtime_error("Unable to _handler->GetId(). Error code: " + WinUtils::HRESULTtoString(hr));

    // retrieve device audio format
    hr = propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &devicePROPVARIANT);
    if (hr != S_OK)
        throw std::runtime_error("Unable to propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat). Error code: " + WinUtils::HRESULTtoString(hr));

    WORD wFormatTag = (reinterpret_cast<WAVEFORMATEX*>(devicePROPVARIANT.blob.pBlobData))->wFormatTag;
    if (wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        auto deviceWaveFormat = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(devicePROPVARIANT.blob.pBlobData);
        _deviceWaveProperties = WaveProperties(
          deviceWaveFormat->Format.nChannels,
          deviceWaveFormat->Format.nSamplesPerSec,
          deviceWaveFormat->Samples.wValidBitsPerSample,
          deviceWaveFormat->Format.wBitsPerSample,
          true);
    } else {
        auto deviceWaveFormat = reinterpret_cast<WAVEFORMATEX*>(devicePROPVARIANT.blob.pBlobData);
        _deviceWaveProperties = WaveProperties(
          deviceWaveFormat->nChannels,
          deviceWaveFormat->nSamplesPerSec,
          deviceWaveFormat->wBitsPerSample,
          deviceWaveFormat->wBitsPerSample,
          true);
    }

    // clear
    PropVariantClear(&devicePROPVARIANT);
    CoTaskMemFree(deviceId);
    propertyStore->Release();
}

Device::~Device() {
    if (_captureClient != nullptr) {
        _captureClient->Release();
    }
    if (_renderClient != nullptr) {
        _renderClient->Release();
    }
}

StreamProperties
Device::openRead(Duration const& requestedDuration, bool isExclusive) {
    if (_handler == nullptr) {
        throw std::logic_error("Device _handler is nullptr");
    }

    // TODO: set to minimum available duration and check if lesser
    HRESULT hr;
    WAVEFORMATEX* deviceMixFormat = nullptr;
    DWORD streamFlags;
    uint32_t fragmentDurationRequested = (uint32_t)(std::chrono::duration_cast<std::chrono::nanoseconds>(requestedDuration).count() / 100);

    switch (getDeviceType()) {
    case DeviceType::RENDER:
        streamFlags = AUDCLNT_STREAMFLAGS_LOOPBACK;
        break;
    case DeviceType::CAPTURE:
        streamFlags = 0;
        break;
    default:
        throw std::logic_error("Unknown device type");
    }

    // creates COM object
    hr = _handler->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&_captureStream);
    if (hr != S_OK)
        throw std::runtime_error("Unable to _handler->Activate(). Error code: " + WinUtils::HRESULTtoString(hr));
    hr = _captureStream->GetMixFormat(&deviceMixFormat);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->GetMixFormat(). Error code:" + WinUtils::HRESULTtoString(hr));

    WaveProperties streamWaveProperties;
    if (deviceMixFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        WAVEFORMATEXTENSIBLE* deviceMixFormatEx = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(deviceMixFormat);
        streamWaveProperties = WaveProperties(
          deviceMixFormatEx->Format.nChannels,
          deviceMixFormatEx->Format.nSamplesPerSec,
          deviceMixFormatEx->Samples.wValidBitsPerSample,
          deviceMixFormatEx->Format.wBitsPerSample,
          false);
    } else {
        streamWaveProperties = WaveProperties(
          deviceMixFormat->nChannels,
          deviceMixFormat->nSamplesPerSec,
          deviceMixFormat->wBitsPerSample,
          deviceMixFormat->wBitsPerSample,
          false);
    }

    hr = _captureStream->Initialize(
      AUDCLNT_SHAREMODE_SHARED,
      streamFlags, //AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST
      fragmentDurationRequested,
      0,
      deviceMixFormat,
      nullptr);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->Initialize(). Error code: " + WinUtils::HRESULTtoString(hr));
    hr = _captureStream->GetService(IID_IAudioCaptureClient, (void**)&_captureClient);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->GetService(). Error code: " + WinUtils::HRESULTtoString(hr));

    /*hEvent = CreateEvent(nullptr, false, false, nullptr);
    //hEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (hEvent == NULL)
    {
        throw std::runtime_error("Unable to create samples ready event: " + GetLastError());
    }
    hr = stream->SetEventHandle(hEvent);
    assert(hr==S_OK);*/

    // get the actual size of the allocated buffer
    std::size_t actualBufferFramesCount = { 0 };
    hr = _captureStream->GetBufferSize(&actualBufferFramesCount);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->GetBufferSize(). Error code: " + WinUtils::HRESULTtoString(hr));
    _bufferFramesCount = actualBufferFramesCount;

    /*=======
    // Grab the entire buffer for the initial fill operation.
    hr = pRenderClient->GetBuffer(_bufferFramesCount, &pData);
    EXIT_ON_ERROR(hr)

    // Load the initial data into the shared buffer.
    hr = pMySource->LoadData(_bufferFramesCount, pData, &flags);
    EXIT_ON_ERROR(hr)

    hr = pRenderClient->ReleaseBuffer(_bufferFramesCount, flags);
    EXIT_ON_ERROR(hr)
    //============*/

    // start device playing/recording
    hr = _captureStream->Start();
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->Start(). Error code: " + WinUtils::HRESULTtoString(hr));

    return StreamProperties {
        actualBufferFramesCount,
        std::move(streamWaveProperties) };
    }

StreamProperties
Device::openWrite(Duration const& requestedDuration, bool isExclusive) {
    if (_handler == nullptr) {
        throw std::logic_error("Device handler is nullptr");
    }

    HRESULT hr = S_OK;
    WAVEFORMATEX* deviceMixFormat = nullptr;
    DWORD streamFlags;
    std::uint32_t fragmentDurationRequested = (uint32_t)(std::chrono::duration_cast<std::chrono::nanoseconds>(requestedDuration).count() / 100);

    switch (getDeviceType()) {
    case DeviceType::RENDER:
        streamFlags = AUDCLNT_STREAMFLAGS_LOOPBACK;
        break;
    case DeviceType::CAPTURE:
        streamFlags = 0;
        break;
    default:
        throw std::logic_error("Unknown device type");
    }

    // creates COM object
    hr = _handler->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&_renderStream));
    if (hr != S_OK)
        throw std::runtime_error("Unable to _handler->Activate(). Error code: " + WinUtils::HRESULTtoString(hr));
    hr = _renderStream->GetMixFormat(&deviceMixFormat);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->GetMixFormat(). Error code: " + WinUtils::HRESULTtoString(hr));
    // TODO: handle WAVEFORMATEX

    WaveProperties streamWaveProperties;
    if (deviceMixFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        WAVEFORMATEXTENSIBLE* deviceMixFormatEx = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(deviceMixFormat);
        streamWaveProperties = WaveProperties(
          deviceMixFormatEx->Format.nChannels,
          deviceMixFormatEx->Format.nSamplesPerSec,
          deviceMixFormatEx->Samples.wValidBitsPerSample,
          deviceMixFormatEx->Format.wBitsPerSample,
          false);
    } else {
        streamWaveProperties = WaveProperties(
          deviceMixFormat->nChannels,
          deviceMixFormat->nSamplesPerSec,
          deviceMixFormat->wBitsPerSample,
          deviceMixFormat->wBitsPerSample,
          false);
    }

    hr = _renderStream->Initialize(
      AUDCLNT_SHAREMODE_SHARED,
      0,
      fragmentDurationRequested,
      0,
      deviceMixFormat,
      nullptr);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->Initialize(). Error code: " + WinUtils::HRESULTtoString(hr));
    hr = _renderStream->GetService(IID_IAudioRenderClient, (void**)&_renderClient);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->GetService(). Error code: " + WinUtils::HRESULTtoString(hr));

    /*hEvent = CreateEvent(nullptr, false, false, nullptr);
    //hEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (hEvent == NULL)
    {
        throw std::runtime_error("Unable to create samples ready event: " + GetLastError());
    }
    hr = stream->SetEventHandle(hEvent);
    assert(hr==S_OK);*/

    // get the actual size of the allocated buffer
    FramesCount actualBufferFramesCount = { 0 };
    hr = _renderStream->GetBufferSize(&actualBufferFramesCount);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->GetBufferSize(). Error code: " + WinUtils::HRESULTtoString(hr));
    _bufferFramesCount = actualBufferFramesCount;

    /*=======
    // Grab the entire buffer for the initial fill operation.
    hr = pRenderClient->GetBuffer(_bufferFramesCount, &pData);
    EXIT_ON_ERROR(hr)

    // Load the initial data into the shared buffer.
    hr = pMySource->LoadData(_bufferFramesCount, pData, &flags);
    EXIT_ON_ERROR(hr)

    hr = pRenderClient->ReleaseBuffer(_bufferFramesCount, flags);
    EXIT_ON_ERROR(hr)
    //============*/

    // start device playing/recording
    hr = _renderStream->Start();
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->Start(). Error code: " + WinUtils::HRESULTtoString(hr));

    return StreamProperties{
        actualBufferFramesCount,
        std::move(streamWaveProperties) };
}

void
Device::closeRead() {
    HRESULT hr = S_OK;
    if (_captureStream != nullptr) {
        hr = _captureStream->Stop();
        if (hr != S_OK)
            throw std::runtime_error("Unable to captureStream->Stop(). Error code: " + WinUtils::HRESULTtoString(hr));
        _captureStream = nullptr;
    }
}

void
Device::closeWrite() {
    HRESULT hr = S_OK;
    if (_renderStream != nullptr) {
        hr = _renderStream->Stop();
        if (hr != S_OK)
            throw std::runtime_error("Unable to captureStream->Stop(). Error code: " + WinUtils::HRESULTtoString(hr));
        _renderStream = nullptr;
    }
}

BufferStatus
Device::readData(Buffer& buffer, std::uint16_t blockAlign) {
    HRESULT hr = S_OK;
    DWORD flags = 0;
    uint32_t packetLength;
    uint32_t numFramesAvailable;
    BYTE* pData;
    std::vector<uint8_t> arr;

    hr = _captureClient->GetNextPacketSize(&packetLength);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->GetNextPacketSize(). Error code: " + WinUtils::HRESULTtoString(hr));

    while (packetLength != 0) {
        // get the available data in the shared buffer.
        hr = _captureClient->GetBuffer(&pData, &numFramesAvailable, &flags, nullptr, nullptr);
        if (hr != S_OK)
            throw std::runtime_error("Unable to captureStream->GetBuffer(). Error code: " + WinUtils::HRESULTtoString(hr));

        if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
            pData = nullptr; // Tell CopyData to write silence.
        }

        // each frame contains number of bytes equal to block align
        // TODO: return true or false?
        buffer.write(pData, numFramesAvailable * blockAlign);

        // release data
        hr = _captureClient->ReleaseBuffer(numFramesAvailable);
        if (hr != S_OK)
            throw std::runtime_error("Unable to captureStream->ReleaseBuffer(). Error code: " + WinUtils::HRESULTtoString(hr));

        // start next capture
        hr = _captureClient->GetNextPacketSize(&packetLength);
        if (hr != S_OK)
            throw std::runtime_error("Unable to captureStream->GetNextPacketSize(). Error code: " + WinUtils::HRESULTtoString(hr));
    }

    return BufferStatus::BufferFilled;
}

bool
Device::writeData(Buffer const& buffer, std::uint16_t blockAlign) {
    if (buffer.getSize() == 0)
        return true;

    HRESULT hr;
    DWORD flags(0);
    uint32_t bufferFramesPadding(0);
    uint32_t numFramesAvailable(0);
    uint8_t* pData = nullptr;

    // TODO: rework
    if (flags == AUDCLNT_BUFFERFLAGS_SILENT)
        return true;

    // See how much buffer space is available.
    hr = _renderStream->GetCurrentPadding(&bufferFramesPadding);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->GetCurrentPadding(). Error code: " + WinUtils::HRESULTtoString(hr));

    numFramesAvailable = _bufferFramesCount - bufferFramesPadding;

    // Grab all the available space in the shared buffer.
    //AUDCLNT_E_BUFFER_ERROR
    hr = _renderClient->GetBuffer(numFramesAvailable, &pData);
    if (hr != S_OK) {
        auto errorText = WinUtils::HRESULTtoString(hr);
        throw std::runtime_error("Unable to captureStream->GetBuffer(). Error code: " + WinUtils::HRESULTtoString(hr));
    }

    // tmp solution
    std::size_t bufferSize = (buffer.getSize() / blockAlign) * blockAlign;
    std::size_t availableSize = numFramesAvailable * blockAlign;
    std::size_t takeoverSize = 0;
    if (availableSize > bufferSize)
        takeoverSize = bufferSize;
    else
        takeoverSize = availableSize;
    buffer.read(pData, takeoverSize);
    numFramesAvailable = takeoverSize / blockAlign;
    // end of tmp solution

    hr = _renderClient->ReleaseBuffer(numFramesAvailable, flags);
    if (hr != S_OK)
        throw std::runtime_error("Unable to captureStream->ReleaseBuffer(). Error code: " + WinUtils::HRESULTtoString(hr));

    return true;
}

String const&
Device::getDescription() const {
    return _description;
}

String const&
Device::getName() const {
    return _name;
}

} // namespace Wasapi
} // namespace DeviceApi
} // namespace CASM
