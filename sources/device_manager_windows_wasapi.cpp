// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#include "device_manager_windows_wasapi.hpp"
#include <cassert>


DeviceManagerWindowsWASAPI::DeviceManagerWindowsWASAPI() {
    HRESULT hr;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    assert(hr==S_OK);

    update();
}

DeviceManagerWindowsWASAPI::~DeviceManagerWindowsWASAPI() {
    CoUninitialize();
}

int DeviceManagerWindowsWASAPI::update() {
    HRESULT hr;

    IMMDeviceCollection *deviceCollection = NULL;
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    IMMDevice* device = NULL;

    //TODO: replace with swap
    deviceList.clear();

    //CLSID_MMDeviceEnumerator or __uuidof(IMMDeviceEnumerator)
    hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
    assert(hr==S_OK);

    // eRender - includes rendering endpoints to collection
    hr = deviceEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &deviceCollection);
    assert(hr==S_OK);
    hr = deviceCollection->GetCount(&deviceCount);
    assert(hr==S_OK);

    deviceList.resize(deviceCount);
    for (uint32_t deviceIndex=0; deviceIndex<deviceCount; deviceIndex++)
    {
        deviceCollection->Item(deviceIndex, &device);
        deviceList[deviceIndex] = DeviceWindowsWASAPI(device, CASM::CAPTURE);
    }

    deviceEnumerator->Release();
    deviceCollection->Release();
    device->Release();

    return 0;
}
