// =============== DESCRIPTION ===============
// ===========================================

#include "device_windows_wasapi.hpp"
#include <thread>


DeviceWindowsWASAPI::DeviceWindowsWASAPI() {
    /*HRESULT hr;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    assert(hr==S_OK);*/
}

DeviceWindowsWASAPI::DeviceWindowsWASAPI(DeviceHandler *device, CASM::DeviceType deviceType) : DeviceTemplate<IMMDevice, IAudioClient*>::DeviceTemplate(device, deviceType){
    HRESULT hr;
    LPWSTR deviceId;
    IPropertyStore* devicePropertyStore;
    PROPVARIANT deviceProperty;
    WAVEFORMATEX deviceWaveProperties;

    // active
    PropVariantInit(&deviceProperty);

    // read device properties
    hr = device->OpenPropertyStore(STGM_READ, &devicePropertyStore);
    assert(hr==S_OK);
    hr = devicePropertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &deviceProperty);
    assert(hr==S_OK);
    deviceWaveProperties = *((WAVEFORMATEX*)deviceProperty.blob.pBlobData);
    waveProperties = WaveProperties(deviceWaveProperties.nChannels, deviceWaveProperties.nSamplesPerSec, deviceWaveProperties.wBitsPerSample);
    devicePropertyStore->GetValue(PKEY_Device_DeviceDesc, &deviceProperty);
    name = deviceProperty.pwszVal;

    hr = device->OpenPropertyStore(STGM_READ, &devicePropertyStore);
    assert(hr==S_OK);
    hr = devicePropertyStore->GetValue(PKEY_Device_FriendlyName, &deviceProperty);
    assert(hr==S_OK);
    description = std::wstring(deviceProperty.pwszVal);

    hr = device->GetId(&deviceId);
    assert(hr==S_OK);

    // clear
    PropVariantClear(&deviceProperty);
    CoTaskMemFree(deviceId);
    devicePropertyStore->Release();
}

DeviceWindowsWASAPI::~DeviceWindowsWASAPI() {

}

int DeviceWindowsWASAPI::open(CASM::Access access, std::chrono::duration<double> fragmentDuration) {
    // checks
    if (active) {
        throw std::logic_error("Device already in use");
        return 1;
    }
    if (handler==nullptr) {
        throw std::logic_error("Device handler is nullptr");
        return 1;
    }

    fragmentDurationRequested = (uint64_t)(std::chrono::duration_cast<std::chrono::nanoseconds>(fragmentDuration).count()/100);
    HRESULT hr;
    WAVEFORMATEX *deviceMixFormat = NULL;
    // REFTIMES per second
    uint64_t fragmentDurationActual;
    uint32_t fragmentFrameCount;

    // creates COM object
    hr = handler->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&stream);
    assert(hr==S_OK);

    hr = stream->GetMixFormat(&deviceMixFormat);
    WaveProperties mixWaveProperties (deviceMixFormat->nChannels, deviceMixFormat->nSamplesPerSec, deviceMixFormat->wBitsPerSample, false);
    assert(hr==S_OK);
    switch (access) {
    case CASM::READ:
        hr = stream->Initialize(
                AUDCLNT_SHAREMODE_SHARED,
                0,//AUDCLNT_STREAMFLAGS_LOOPBACK,
                fragmentDurationRequested,
                0,
                deviceMixFormat,
                NULL);
        assert(hr==S_OK);

        hr = stream->GetService(IID_IAudioCaptureClient, (void**)&captureClient);
        assert(hr==S_OK);
        break;
    case CASM::WRITE:
        hr = stream->Initialize(
                AUDCLNT_SHAREMODE_SHARED,
                0,
                fragmentDurationRequested,
                0,
                deviceMixFormat,
                NULL);
        assert(hr==S_OK);

        hr = stream->GetService(IID_IAudioRenderClient, (void**)&renderClient);
        assert(hr==S_OK);
        break;
    default:
        assert(false);
    }

    // get the actual size of the allocated buffer
    hr = stream->GetBufferSize(&fragmentFrameCount);
    assert(hr==S_OK);
    // calculate the actual duration of the allocated buffer
    fragmentDurationActual = (double)fragmentDurationRequested * fragmentFrameCount / deviceMixFormat->nSamplesPerSec;
    // create buffer
    buffer = Buffer(mixWaveProperties, fragmentDuration);

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

Buffer DeviceWindowsWASAPI::read() {
    HRESULT hr;
    DWORD flags = 0;
    uint32_t packetLength;
    uint32_t numFramesAvailable;
    BYTE *pData;
    std::vector<uint8_t> arr;

    // Each loop fills about half of the shared buffer.
    if(flags != AUDCLNT_BUFFERFLAGS_SILENT) {
        std::this_thread::sleep_for(buffer.getDuration()/2);

        hr = captureClient->GetNextPacketSize(&packetLength);
        assert(hr==S_OK);

        while (packetLength!=0) {
            // get the available data in the shared buffer.
            hr = captureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
            assert(hr==S_OK);

            if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
                pData = nullptr;  // Tell CopyData to write silence.
            }

            buffer.write<uint8_t>(pData, numFramesAvailable);

            // release data
            hr = captureClient->ReleaseBuffer(numFramesAvailable);
            assert(hr==S_OK);

            // start next capture
            hr = captureClient->GetNextPacketSize(&packetLength);
            assert(hr==S_OK);
        }
    }

    return buffer;
}

int DeviceWindowsWASAPI::write(Buffer data) {
    HRESULT hr;
    DWORD flags = 0;
    uint32_t numFramesPadding;
    uint32_t numFramesAvailable;
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