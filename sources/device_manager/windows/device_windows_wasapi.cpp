// =============== DESCRIPTION ===============
// ===========================================

#include "device_windows_wasapi.hpp"
#include <thread>


DeviceWindowsWASAPI::DeviceWindowsWASAPI() {
    stream = nullptr;
    captureClient = nullptr;
    renderClient = nullptr;
    /*HRESULT hr;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    assert(hr==S_OK);*/
}

DeviceWindowsWASAPI::~DeviceWindowsWASAPI() {
    /*captureClient->Release();
    renderClient->Release();*/
}

DeviceWindowsWASAPI::DeviceWindowsWASAPI(void* device, CASM::DeviceType deviceType) : DeviceTemplate<IMMDevice>::DeviceTemplate(device, deviceType){
    HRESULT hr;
    LPWSTR deviceId;
    IPropertyStore* propertyStore;
    PROPVARIANT deviceProperty{};
    WAVEFORMATEX deviceWaveProperties{};

    // active
    PropVariantInit(&deviceProperty);

    // read device properties
    hr = handler->OpenPropertyStore(STGM_READ, &propertyStore);
    assert(hr==S_OK);
    hr = propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &deviceProperty);
    assert(hr==S_OK);
    deviceWaveProperties = *((WAVEFORMATEX*)deviceProperty.blob.pBlobData);
    DeviceWindowsWASAPI::deviceWaveProperties = WaveProperties(deviceWaveProperties.nChannels, deviceWaveProperties.nSamplesPerSec, deviceWaveProperties.wBitsPerSample);
    propertyStore->GetValue(PKEY_Device_DeviceDesc, &deviceProperty);
    name = deviceProperty.pwszVal;

    hr = handler->OpenPropertyStore(STGM_READ, &propertyStore);
    assert(hr==S_OK);
    hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &deviceProperty);
    assert(hr==S_OK);
    description = std::wstring(deviceProperty.pwszVal);

    hr = handler->GetId(&deviceId);
    assert(hr==S_OK);

    // clear
    PropVariantClear(&deviceProperty);
    CoTaskMemFree(deviceId);
    propertyStore->Release();
}

int DeviceWindowsWASAPI::open(CASM::Access access, std::chrono::duration<double> fragmentDuration) {
    // checks
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
    uint32_t fragmentFrameCount;

    fragmentDurationRequested = (uint64_t)(std::chrono::duration_cast<std::chrono::nanoseconds>(fragmentDuration).count()/100);
    switch(type) {
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
    hr = handler->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&stream);
    assert(hr==S_OK);

    hr = stream->GetMixFormat(&deviceMixFormat);
    streamWaveProperties = WaveProperties(deviceMixFormat->nChannels, deviceMixFormat->nSamplesPerSec, deviceMixFormat->wBitsPerSample, false);
    assert(hr==S_OK);
    switch (access) {
    case CASM::READ:
        hr = stream->Initialize(
                AUDCLNT_SHAREMODE_SHARED,
                streamFlags,
                //AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST
                fragmentDurationRequested,
                0,//0,
                deviceMixFormat,
                nullptr);
        assert(hr==S_OK);

        hr = stream->GetService(IID_IAudioCaptureClient, (void**)&captureClient);
        assert(hr==S_OK);
        if (hr!=S_OK) throw std::runtime_error("Unable to GetService");
        break;
    case CASM::WRITE:
        hr = stream->Initialize(
                AUDCLNT_SHAREMODE_SHARED,
                0,
                fragmentDurationRequested,
                0,
                deviceMixFormat,
                nullptr);
        assert(hr==S_OK);

        hr = stream->GetService(IID_IAudioRenderClient, (void**)&renderClient);
        assert(hr==S_OK);
        break;
    default:
        throw std::runtime_error("Unknown device access");
    }

    /*hEvent = CreateEvent(nullptr, false, false, nullptr);
    //hEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (hEvent == NULL)
    {
        throw std::runtime_error("Unable to create samples ready event: " + GetLastError());
    }
    hr = stream->SetEventHandle(hEvent);
    assert(hr==S_OK);*/

    // get the actual size of the allocated buffer
    hr = stream->GetBufferSize(&fragmentFrameCount);
    assert(hr==S_OK);
    // create buffer
    buffer = Buffer(streamWaveProperties, fragmentFrameCount);

    /*=======
    // Grab the entire buffer for the initial fill operation.
    hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
    EXIT_ON_ERROR(hr)

    // Load the initial data into the shared buffer.
    hr = pMySource->LoadData(bufferFrameCount, pData, &flags);
    EXIT_ON_ERROR(hr)

    hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
    EXIT_ON_ERROR(hr)
    //============*/

    // start device playing/recording
    hr = stream->Start();
    assert(hr==S_OK);
    active = true;

    return 0;
}

int DeviceWindowsWASAPI::close() {
    HRESULT hr;

    // stop playing/recording
    hr = stream->Stop();
    assert(hr==S_OK);
    active = false;

    return 0;
}

bool DeviceWindowsWASAPI::read(Buffer& buffer)
{
    HRESULT hr;
    DWORD flags = 0;
    uint32_t packetLength;
    uint32_t numFramesAvailable;
    BYTE *pData;
    std::vector<uint8_t> arr;

    // Each loop fills about half of the shared buffer.
    std::this_thread::sleep_for(buffer.getDuration()/2);
    //hrs = WaitForSingleObject(hEvent, INFINITE);

    hr = captureClient->GetNextPacketSize(&packetLength);
    assert(hr==S_OK);

    while (packetLength!=0) {
        // get the available data in the shared buffer.
        hr = captureClient->GetBuffer(&pData, &numFramesAvailable, &flags, nullptr, nullptr);
        assert(hr==S_OK);

        if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
            pData = nullptr;  // Tell CopyData to write silence.
        }

        // each frame contains number of bytes equal to block align
        // TODO: return true or false?
        buffer.write<uint8_t>(pData, numFramesAvailable*streamWaveProperties.getBlockAlign());

        // release data
        hr = captureClient->ReleaseBuffer(numFramesAvailable);
        assert(hr==S_OK);

        // start next capture
        hr = captureClient->GetNextPacketSize(&packetLength);
        assert(hr==S_OK);
    }

    return true;
}

Buffer DeviceWindowsWASAPI::read() {
    HRESULT hr;
    DWORD flags = 0;
    uint32_t packetLength;
    uint32_t numFramesAvailable;
    BYTE *pData;
    std::vector<uint8_t> arr;

    // Each loop fills about half of the shared buffer.
    std::this_thread::sleep_for(buffer.getDuration()/2);
    //hrs = WaitForSingleObject(hEvent, INFINITE);

    hr = captureClient->GetNextPacketSize(&packetLength);
    assert(hr==S_OK);

    while (packetLength!=0) {
        // get the available data in the shared buffer.
        hr = captureClient->GetBuffer(&pData, &numFramesAvailable, &flags, nullptr, nullptr);
        assert(hr==S_OK);

        if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
            pData = nullptr;  // Tell CopyData to write silence.
        }

        // each frame contains number of bytes equal to block align
        // TODO: move to stream class? is buffer should be assigned to stream?
        buffer.write<uint8_t>(pData, numFramesAvailable*streamWaveProperties.getBlockAlign());

        // release data
        hr = captureClient->ReleaseBuffer(numFramesAvailable);
        assert(hr==S_OK);

        // start next capture
        hr = captureClient->GetNextPacketSize(&packetLength);
        assert(hr==S_OK);
    }

    return buffer;
}

bool DeviceWindowsWASAPI::write(Buffer buffer) {
    HRESULT hr;
    DWORD flags(0);
    uint32_t numFramesPadding(0);
    uint32_t numFramesAvailable(0);
    uint8_t *pData;

    if (flags != AUDCLNT_BUFFERFLAGS_SILENT)
    {
        // Sleep for half the buffer duration.
        std::this_thread::sleep_for(buffer.getDuration());
        //Sleep((DWORD)(buffer.getDuration()/fragmentDurationRequested*1000/2));

        // See how much buffer space is available.
        hr = stream->GetCurrentPadding(&numFramesPadding);
        assert(hr==S_OK);

        //numFramesAvailable = buffer.getSize() - numFramesPadding;

        // Grab all the available space in the shared buffer.
        hr = renderClient->GetBuffer(numFramesAvailable, &pData);
        assert(hr==S_OK);

        // Get next 1/2-second of data from the audio source.
        //hr = pMySource->LoadData(numFramesAvailable, pData, &flags);
        //assert(hr==S_OK);
        //std::fill( pData, pData + numFramesAvailable, 3 );
        //std::copy(buffer.begin(), src.begin()+count, dest);
        //TODO: add
        //data.read(pData, numFramesAvailable);

        hr = renderClient->ReleaseBuffer(numFramesAvailable, flags);
        assert(hr==S_OK);
    }

    return 0;
}
