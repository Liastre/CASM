// =============== DESCRIPTION ===============
// ===========================================

#include "device_windows_wasapi.hpp"
#include <thread>
#include <sstream>


namespace CASM {

DeviceWindowsWASAPI::DeviceWindowsWASAPI() {
    stream = nullptr;
    captureClient = nullptr;
    renderClient = nullptr;
    /*HRESULT hr;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    assert(hr==S_OK);*/
}


DeviceWindowsWASAPI::DeviceWindowsWASAPI(void *device, CASM::DeviceType deviceType) :DeviceBase< IMMDevice >::DeviceBase(device, deviceType) {
    // init
    stream = nullptr;
    captureClient = nullptr;
    renderClient = nullptr;

    // local variables
    HRESULT hr;
    LPWSTR deviceId;
    IPropertyStore *propertyStore;
    PROPVARIANT devicePROPVARIANT{};
    WAVEFORMATEX deviceWAVEFORMATEX{};

    // active
    PropVariantInit(&devicePROPVARIANT);

    // read device properties
    hr = handler->OpenPropertyStore(STGM_READ, &propertyStore);
    if (hr!=S_OK) throw std::runtime_error("Unable to handler->OpenPropertyStore(). Error code: " + WinUtils::HRESULTtoString(hr));

    // retrieve device audio format
    hr = propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &devicePROPVARIANT);
    if (hr!=S_OK) throw std::runtime_error("Unable to propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat). Error code: " + WinUtils::HRESULTtoString(hr));
    deviceWAVEFORMATEX = *((WAVEFORMATEX *) devicePROPVARIANT.blob.pBlobData);
    deviceWaveProperties = WaveProperties(deviceWAVEFORMATEX.nChannels, deviceWAVEFORMATEX.nSamplesPerSec, deviceWAVEFORMATEX.wBitsPerSample);

    // retrieve device description
    propertyStore->GetValue(PKEY_Device_DeviceDesc, &devicePROPVARIANT);
    name = devicePROPVARIANT.pwszVal;

    // retrieve device name
    hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &devicePROPVARIANT);
    if (hr!=S_OK) throw std::runtime_error("Unable to propertyStore->GetValue(PKEY_Device_FriendlyName). Error code: " + WinUtils::HRESULTtoString(hr));
    description = std::wstring(devicePROPVARIANT.pwszVal);

    hr = handler->GetId(&deviceId);
    if (hr!=S_OK) throw std::runtime_error("Unable to handler->GetId(). Error code: " + WinUtils::HRESULTtoString(hr));

    // clear
    PropVariantClear(&devicePROPVARIANT);
    CoTaskMemFree(deviceId);
    propertyStore->Release();
}


DeviceWindowsWASAPI::~DeviceWindowsWASAPI() {
    if (captureClient!=nullptr) {
        captureClient->Release();
    }
    if (renderClient!=nullptr) {
        renderClient->Release();
    }
}


Buffer DeviceWindowsWASAPI::open(std::chrono::duration< double > duration) {
    bufferDuration = duration;

    // checks
    if (bufferDuration==std::chrono::duration< double >::zero()) {
        throw std::logic_error("Buffer duration is zero");
    }
    if (active) {
        throw std::logic_error("Device already in use");
    }
    if (handler==nullptr) {
        throw std::logic_error("Device handler is nullptr");
    }

    // variables
    HRESULT hr;
    WAVEFORMATEX *deviceMixFormat = nullptr;
    DWORD streamFlags;
    uint32_t tmpBufferFramesCount;
    uint32_t fragmentDurationRequested = (uint32_t) (std::chrono::duration_cast< std::chrono::nanoseconds >(bufferDuration).count()/100);

    switch (type) {
    case CASM::RENDER:
        streamFlags = AUDCLNT_STREAMFLAGS_LOOPBACK;
        break;
    case CASM::CAPTURE:
        streamFlags = 0;
        break;
    default:
        throw std::logic_error("Unknown device type");
    }

    // creates COM object
    hr = handler->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void **) &stream);
    if (hr!=S_OK) throw std::runtime_error("Unable to handler->Activate(). Error code: " + WinUtils::HRESULTtoString(hr));

    hr = stream->GetMixFormat(&deviceMixFormat);
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetMixFormat(). Error code:" + WinUtils::HRESULTtoString(hr));
    streamWaveProperties = WaveProperties(deviceMixFormat->nChannels, deviceMixFormat->nSamplesPerSec, deviceMixFormat->wBitsPerSample, false);

    hr = stream->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            streamFlags, //AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST
            fragmentDurationRequested,
            0,
            deviceMixFormat,
            nullptr
    );
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->Initialize(). Error code: " + WinUtils::HRESULTtoString(hr));

    hr = stream->GetService(IID_IAudioCaptureClient, (void **) &captureClient);
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetService(). Error code: " + WinUtils::HRESULTtoString(hr));

    /*hEvent = CreateEvent(nullptr, false, false, nullptr);
    //hEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (hEvent == NULL)
    {
        throw std::runtime_error("Unable to create samples ready event: " + GetLastError());
    }
    hr = stream->SetEventHandle(hEvent);
    assert(hr==S_OK);*/

    // get the actual size of the allocated buffer
    hr = stream->GetBufferSize(&tmpBufferFramesCount);
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetBufferSize(). Error code: " + WinUtils::HRESULTtoString(hr));
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
    hr = stream->Start();
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->Start(). Error code: " + WinUtils::HRESULTtoString(hr));
    active = true;

    return Buffer(streamWaveProperties, duration);
}


bool DeviceWindowsWASAPI::open(Buffer buffer) {
    bufferDuration = buffer.getDuration();

    // checks
    if (bufferDuration==std::chrono::duration< double >::zero()) {
        throw std::logic_error("Buffer duration is zero");
    }
    if (active) {
        throw std::logic_error("Device already in use");
    }
    if (handler==nullptr) {
        throw std::logic_error("Device handler is nullptr");
    }

    // variables
    HRESULT hr;
    WAVEFORMATEX *deviceMixFormat = nullptr;
    DWORD streamFlags;
    uint32_t tmpBufferFramesCount;
    uint32_t fragmentDurationRequested = (uint32_t) (std::chrono::duration_cast< std::chrono::nanoseconds >(bufferDuration).count()/100);

    switch (type) {
    case CASM::RENDER:
        streamFlags = AUDCLNT_STREAMFLAGS_LOOPBACK;
        break;
    case CASM::CAPTURE:
        streamFlags = 0;
        break;
    default:
        throw std::logic_error("Unknown device type");
    }

    // creates COM object
    hr = handler->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void **) &stream);
    if (hr!=S_OK) throw std::runtime_error("Unable to handler->Activate(). Error code: " + WinUtils::HRESULTtoString(hr));

    hr = stream->GetMixFormat(&deviceMixFormat);
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetMixFormat(). Error code: " + WinUtils::HRESULTtoString(hr));
    streamWaveProperties = WaveProperties(deviceMixFormat->nChannels, deviceMixFormat->nSamplesPerSec, deviceMixFormat->wBitsPerSample, false);
    hr = stream->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, fragmentDurationRequested, 0, deviceMixFormat, nullptr);
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->Initialize(). Error code: " + WinUtils::HRESULTtoString(hr));

    hr = stream->GetService(IID_IAudioRenderClient, (void **) &renderClient);
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetService(). Error code: " + WinUtils::HRESULTtoString(hr));

    /*hEvent = CreateEvent(nullptr, false, false, nullptr);
    //hEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (hEvent == NULL)
    {
        throw std::runtime_error("Unable to create samples ready event: " + GetLastError());
    }
    hr = stream->SetEventHandle(hEvent);
    assert(hr==S_OK);*/

    // get the actual size of the allocated buffer
    hr = stream->GetBufferSize(&tmpBufferFramesCount);
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetBufferSize(). Error code: " + WinUtils::HRESULTtoString(hr));
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
    hr = stream->Start();
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->Start(). Error code: " + WinUtils::HRESULTtoString(hr));
    active = true;
}


void DeviceWindowsWASAPI::close() {
    if (!active) {
        return;
    }
    HRESULT hr;

    // stop playing/recording
    hr = stream->Stop();
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->Stop(). Error code: " + WinUtils::HRESULTtoString(hr));
    active = false;
}


bool DeviceWindowsWASAPI::read(Buffer &buffer) {
    HRESULT hr;
    DWORD flags = 0;
    uint32_t packetLength;
    uint32_t numFramesAvailable;
    BYTE *pData;
    std::vector< uint8_t > arr;

    // Each loop fills about half of the shared buffer.
    //std::this_thread::sleep_for(buffer.getDuration());
    //hrs = WaitForSingleObject(hEvent, INFINITE);

    hr = captureClient->GetNextPacketSize(&packetLength);
    if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetNextPacketSize(). Error code: " + WinUtils::HRESULTtoString(hr));

    while (packetLength!=0) {
        // get the available data in the shared buffer.
        hr = captureClient->GetBuffer(&pData, &numFramesAvailable, &flags, nullptr, nullptr);
        if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetBuffer(). Error code: " + WinUtils::HRESULTtoString(hr));

        if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
            pData = nullptr;  // Tell CopyData to write silence.
        }

        // each frame contains number of bytes equal to block align
        // TODO: return true or false?
        buffer.write< uint8_t >(pData, numFramesAvailable*streamWaveProperties.getBlockAlign());

        // release data
        hr = captureClient->ReleaseBuffer(numFramesAvailable);
        if (hr!=S_OK) throw std::runtime_error("Unable to stream->ReleaseBuffer(). Error code: " + WinUtils::HRESULTtoString(hr));

        // start next capture
        hr = captureClient->GetNextPacketSize(&packetLength);
        if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetNextPacketSize(). Error code: " + WinUtils::HRESULTtoString(hr));
    }

    return true;
}


bool DeviceWindowsWASAPI::write(Buffer buffer) {
    HRESULT hr;
    DWORD flags(0);
    uint32_t bufferFramesPadding(0);
    uint32_t numFramesAvailable(0);
    uint8_t *pData;

    if (flags!=AUDCLNT_BUFFERFLAGS_SILENT) {
        // See how much buffer space is available.
        hr = stream->GetCurrentPadding(&bufferFramesPadding);
        if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetCurrentPadding(). Error code: " + WinUtils::HRESULTtoString(hr));

        numFramesAvailable = bufferFramesCount-bufferFramesPadding;

        // Grab all the available space in the shared buffer.
        hr = renderClient->GetBuffer(numFramesAvailable, &pData);
        if (hr!=S_OK) throw std::runtime_error("Unable to stream->GetBuffer(). Error code: " + WinUtils::HRESULTtoString(hr));

        // Get next 1/2-second of data from the audio source.
        //hr = pMySource->LoadData(numFramesAvailable, pData, &flags);
        //assert(hr==S_OK);
        //std::fill( pData, pData + numFramesAvailable, 3 );
        //std::copy(buffer.begin(), src.begin()+count, dest);
        buffer.read(pData, numFramesAvailable*streamWaveProperties.getBlockAlign());

        hr = renderClient->ReleaseBuffer(numFramesAvailable, flags);
        if (hr!=S_OK) throw std::runtime_error("Unable to stream->ReleaseBuffer(). Error code: " + WinUtils::HRESULTtoString(hr));
    }

    return true;
}


bool DeviceWindowsWASAPI::isAvailable() {
    return true;
}

}