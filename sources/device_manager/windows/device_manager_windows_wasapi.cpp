// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#include "device_manager_windows_wasapi.hpp"
#include "windows_wasapi.hpp"
#include <cassert>


namespace CASM {

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
    IMMDevice *device = NULL;
    uint32_t deviceCollectionSize;

    //TODO: replace with swap
    deviceList.clear();

    //CLSID_MMDeviceEnumerator or __uuidof(IMMDeviceEnumerator)
    hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
    assert(hr==S_OK);

    // eCapture - includes capture endpoints to collection
    hr = deviceEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &deviceCollection);
    assert(hr==S_OK);
    hr = deviceCollection->GetCount(&deviceCollectionSize);
    assert(hr==S_OK);
    for (uint32_t deviceIndex = 0; deviceIndex < deviceCollectionSize; deviceIndex++) {
        deviceCollection->Item(deviceIndex, &device);
        deviceList.push_back(Device(device, CASM::CAPTURE));
    }
    deviceCollection->Release();

    // eRender - includes rendering endpoints to collection
    hr = deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &deviceCollection);
    assert(hr==S_OK);
    hr = deviceCollection->GetCount(&deviceCollectionSize);
    assert(hr==S_OK);
    for (uint32_t deviceIndex = 0; deviceIndex < deviceCollectionSize; deviceIndex++) {
        deviceCollection->Item(deviceIndex, &device);
        deviceList.push_back(Device(device, CASM::RENDER));
    }
    deviceCollection->Release();

    deviceCount = (uint_fast32_t) deviceList.size();
    deviceEnumerator->Release();
    device->Release();

    return 0;
}

}