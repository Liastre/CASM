/**
    @file device_windows_wasapi.cpp
    @copyright LGPLv3
    @brief definition for DeviceWindowsWASAPI class
**/

#include "device_windows_wasapi.hpp"
#include <thread>


namespace CASM {

DeviceWindowsWASAPI::DeviceWindowsWASAPI() {
    /*HRESULT hr;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    assert(hr==S_OK);*/
}


DeviceWindowsWASAPI::DeviceWindowsWASAPI(void * device, CASM::DeviceType deviceType) :DeviceBase< IMMDevice >::DeviceBase(device, deviceType) {
    // local variables
    HRESULT hr;
    LPWSTR deviceId;
    IPropertyStore * propertyStore;
    PROPVARIANT devicePROPVARIANT {};
    WAVEFORMATEX deviceWAVEFORMATEX {};

    // active
    PropVariantInit(&devicePROPVARIANT);

    // read device properties
    hr = handler->OpenPropertyStore(STGM_READ, &propertyStore);
    if (hr!=S_OK) throw std::runtime_error("Unable to handler->OpenPropertyStore(). Error code: "+WinUtils::HRESULTtoString(hr));

    // retrieve device audio format
    hr = propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &devicePROPVARIANT);
    if (hr!=S_OK)
        throw std::runtime_error("Unable to propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat). Error code: "+WinUtils::HRESULTtoString(hr));
    deviceWAVEFORMATEX = *((WAVEFORMATEX *) devicePROPVARIANT.blob.pBlobData);
    deviceWaveProperties = WaveProperties(deviceWAVEFORMATEX.nChannels, deviceWAVEFORMATEX.nSamplesPerSec, deviceWAVEFORMATEX.wBitsPerSample);

    // retrieve device description
    propertyStore->GetValue(PKEY_Device_DeviceDesc, &devicePROPVARIANT);
    name = devicePROPVARIANT.pwszVal;

    // retrieve device name
    hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &devicePROPVARIANT);
    if (hr!=S_OK) throw std::runtime_error("Unable to propertyStore->GetValue(PKEY_Device_FriendlyName). Error code: "+WinUtils::HRESULTtoString(hr));
    description = std::wstring(devicePROPVARIANT.pwszVal);

    hr = handler->GetId(&deviceId);
    if (hr!=S_OK) throw std::runtime_error("Unable to handler->GetId(). Error code: "+WinUtils::HRESULTtoString(hr));

    // valid device
    _isValid = {true};

    // clear
    PropVariantClear(&devicePROPVARIANT);
    CoTaskMemFree(deviceId);
    propertyStore->Release();
}


DeviceWindowsWASAPI::~DeviceWindowsWASAPI() {
    if (_captureClient!=nullptr) {
        _captureClient->Release();
    }
    if (_renderClient!=nullptr) {
        _renderClient->Release();
    }
}


bool DeviceWindowsWASAPI::openCaptureStream(Duration const & duration, Buffer & buffer) {
    // TODO: set to minimum available duration and check if lesser
    bufferDuration = duration;

    // checks
    if (bufferDuration==Duration::zero()) throw std::logic_error("Buffer duration is zero");
    if (handler==nullptr) throw std::logic_error("Device handler is nullptr");

    // variables
    HRESULT hr;
    WAVEFORMATEX * deviceMixFormat = nullptr;
    DWORD streamFlags;
    uint32_t tmpBufferFramesCount;
    uint32_t fragmentDurationRequested = (uint32_t) (std::chrono::duration_cast< std::chrono::nanoseconds >(bufferDuration).count()/100);

    switch (type) {
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
    hr = handler->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void **) &_captureStream);
    if (hr!=S_OK) throw std::runtime_error("Unable to handler->Activate(). Error code: "+WinUtils::HRESULTtoString(hr));

    hr = captureStream->GetMixFormat(&deviceMixFormat);
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetMixFormat(). Error code:"+WinUtils::HRESULTtoString(hr));
    _streamWaveProperties = WaveProperties(deviceMixFormat->nChannels, deviceMixFormat->nSamplesPerSec, deviceMixFormat->wBitsPerSample, false);

    hr = _captureStream->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            streamFlags, //AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST
            fragmentDurationRequested,
            0,
            deviceMixFormat,
            nullptr
    );
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->Initialize(). Error code: "+WinUtils::HRESULTtoString(hr));

    hr = _captureStream->GetService(IID_IAudioCaptureClient, (void **) &_captureClient);
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetService(). Error code: "+WinUtils::HRESULTtoString(hr));

    /*hEvent = CreateEvent(nullptr, false, false, nullptr);
    //hEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (hEvent == NULL)
    {
        throw std::runtime_error("Unable to create samples ready event: " + GetLastError());
    }
    hr = stream->SetEventHandle(hEvent);
    assert(hr==S_OK);*/

    // get the actual size of the allocated buffer
    hr = _captureStream->GetBufferSize(&tmpBufferFramesCount);
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetBufferSize(). Error code: "+WinUtils::HRESULTtoString(hr));
    bufferFramesCount = tmpBufferFramesCount;

    /*=======
    // Grab the entire buffer for the initial fill operation.
    hr = pRenderClient->GetBuffer(bufferFramesCount, &pData);
    EXIT_ON_ERROR(hr)

    // Load the initial data into the shared buffer.
    hr = pMySource->LoadData(bufferFramesCount, pData, &flags);
    EXIT_ON_ERROR(hr)

    hr = pRenderClient->ReleaseBuffer(bufferFramesCount, flags);
    EXIT_ON_ERROR(hr)
    //============*/

    // start device playing/recording
    hr = _captureStream->Start();
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->Start(). Error code: "+WinUtils::HRESULTtoString(hr));
    _active = true;

    buffer = Buffer(_streamWaveProperties, duration);

    return true;
}


bool DeviceWindowsWASAPI::openRenderStream(Buffer const & buffer) {
    bufferDuration = buffer.getDuration();

    // checks
    if (bufferDuration==Duration::zero()) {
        throw std::logic_error("Buffer duration is zero");
    }
    if (_active) {
        throw std::logic_error("Device already in use");
    }
    if (handler==nullptr) {
        throw std::logic_error("Device handler is nullptr");
    }

    // variables
    HRESULT hr;
    WAVEFORMATEX * deviceMixFormat = nullptr;
    DWORD streamFlags;
    uint32_t tmpBufferFramesCount;
    uint32_t fragmentDurationRequested = (uint32_t) (std::chrono::duration_cast< std::chrono::nanoseconds >(bufferDuration).count()/100);

    switch (type) {
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
    hr = handler->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void **) &_renderStream);
    if (hr!=S_OK) throw std::runtime_error("Unable to handler->Activate(). Error code: "+WinUtils::HRESULTtoString(hr));
    hr = _renderStream->GetMixFormat(&deviceMixFormat);
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetMixFormat(). Error code: "+WinUtils::HRESULTtoString(hr));
    _streamWaveProperties = WaveProperties(deviceMixFormat->nChannels, deviceMixFormat->nSamplesPerSec, deviceMixFormat->wBitsPerSample, false);
    hr = _renderStream->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            0,
            fragmentDurationRequested,
            0,
            deviceMixFormat,
            nullptr
    );
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->Initialize(). Error code: "+WinUtils::HRESULTtoString(hr));
    hr = _renderStream->GetService(IID_IAudioRenderClient, (void **) &_renderClient);
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetService(). Error code: "+WinUtils::HRESULTtoString(hr));

    /*hEvent = CreateEvent(nullptr, false, false, nullptr);
    //hEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (hEvent == NULL)
    {
        throw std::runtime_error("Unable to create samples ready event: " + GetLastError());
    }
    hr = stream->SetEventHandle(hEvent);
    assert(hr==S_OK);*/

    // get the actual size of the allocated buffer
    hr = _renderStream->GetBufferSize(&tmpBufferFramesCount);
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetBufferSize(). Error code: "+WinUtils::HRESULTtoString(hr));
    bufferFramesCount = tmpBufferFramesCount;

    /*=======
    // Grab the entire buffer for the initial fill operation.
    hr = pRenderClient->GetBuffer(bufferFramesCount, &pData);
    EXIT_ON_ERROR(hr)

    // Load the initial data into the shared buffer.
    hr = pMySource->LoadData(bufferFramesCount, pData, &flags);
    EXIT_ON_ERROR(hr)

    hr = pRenderClient->ReleaseBuffer(bufferFramesCount, flags);
    EXIT_ON_ERROR(hr)
    //============*/

    // start device playing/recording
    hr = _renderStream->Start();
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->Start(). Error code: "+WinUtils::HRESULTtoString(hr));
    _active = true;

    return true;
}


void DeviceWindowsWASAPI::closeCaptureStream() {
    if (!_active) {
        return;
    }
    HRESULT hr;

    if (_captureStream!=nullptr) {
        hr = _captureStream->Stop();
        if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->Stop(). Error code: "+WinUtils::HRESULTtoString(hr));
        _captureStream = nullptr;
    }

    _active = false;
}


void DeviceWindowsWASAPI::closeRenderStream() {
    HRESULT hr;

    if (_renderStream!=nullptr) {
        hr = _renderStream->Stop();
        if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->Stop(). Error code: "+WinUtils::HRESULTtoString(hr));
        _renderStream = nullptr;
    }
}


bool DeviceWindowsWASAPI::read(Buffer & buffer) {
    HRESULT hr;
    DWORD flags = 0;
    uint32_t packetLength;
    uint32_t numFramesAvailable;
    BYTE * pData;
    std::vector< uint8_t > arr;

    // Each loop fills about half of the shared buffer.
    //std::this_thread::sleep_for(buffer.getDuration());
    //hrs = WaitForSingleObject(hEvent, INFINITE);

    hr = _captureClient->GetNextPacketSize(&packetLength);
    if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetNextPacketSize(). Error code: "+WinUtils::HRESULTtoString(hr));

    while (packetLength!=0) {
        // get the available data in the shared buffer.
        hr = _captureClient->GetBuffer(&pData, &numFramesAvailable, &flags, nullptr, nullptr);
        if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetBuffer(). Error code: "+WinUtils::HRESULTtoString(hr));

        if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
            pData = nullptr;  // Tell CopyData to write silence.
        }

        // each frame contains number of bytes equal to block align
        // TODO: return true or false?
        buffer.write(pData, numFramesAvailable*_streamWaveProperties.getBlockAlign());

        // release data
        hr = _captureClient->ReleaseBuffer(numFramesAvailable);
        if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->ReleaseBuffer(). Error code: "+WinUtils::HRESULTtoString(hr));

        // start next capture
        hr = _captureClient->GetNextPacketSize(&packetLength);
        if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetNextPacketSize(). Error code: "+WinUtils::HRESULTtoString(hr));
    }

    return true;
}


//TODO: pass buffer by const ref
bool DeviceWindowsWASAPI::write(Buffer const & buffer) {
    HRESULT hr;
    DWORD flags(0);
    uint32_t bufferFramesPadding(0);
    uint32_t numFramesAvailable(0);
    uint8_t * pData;

    if (flags!=AUDCLNT_BUFFERFLAGS_SILENT) {
        // See how much buffer space is available.
        hr = _renderStream->GetCurrentPadding(&bufferFramesPadding);
        if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetCurrentPadding(). Error code: "+WinUtils::HRESULTtoString(hr));

        numFramesAvailable = bufferFramesCount-bufferFramesPadding;

        // Grab all the available space in the shared buffer.
        hr = _renderClient->GetBuffer(numFramesAvailable, &pData);
        if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->GetBuffer(). Error code: "+WinUtils::HRESULTtoString(hr));

        // Get next 1/2-second of data from the audio source.
        //hr = pMySource->LoadData(numFramesAvailable, pData, &flags);
        //assert(hr==S_OK);
        //std::fill( pData, pData + numFramesAvailable, 3 );
        //std::copy(buffer.begin(), src.begin()+count, dest);
        buffer.read(pData, numFramesAvailable*_streamWaveProperties.getBlockAlign());

        hr = _renderClient->ReleaseBuffer(numFramesAvailable, flags);
        if (hr!=S_OK) throw std::runtime_error("Unable to captureStream->ReleaseBuffer(). Error code: "+WinUtils::HRESULTtoString(hr));
    }

    return true;
}


bool DeviceWindowsWASAPI::isAvailable() const {
    return true;
}

}