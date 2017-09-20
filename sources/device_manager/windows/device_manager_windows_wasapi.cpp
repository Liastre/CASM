// =============== DESCRIPTION ===============
// Created on 22 May.
// ===========================================

#include "device_manager_windows_wasapi.hpp"
#include "windows_wasapi.hpp"
#include <cassert>


namespace CASM {

DeviceManagerWindowsWASAPI::DeviceManagerWindowsWASAPI() {
    HRESULT hr;
    hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (hr!=S_OK) throw std::runtime_error("Unable to CoInitializeEx(). Error code: " + WinUtils::HRESULTtoString(hr));

    update();
}


DeviceManagerWindowsWASAPI::~DeviceManagerWindowsWASAPI() {
    CoUninitialize();
}


int DeviceManagerWindowsWASAPI::update() {
    HRESULT hr;

    IMMDeviceCollection *deviceCollection = nullptr;
    IMMDeviceEnumerator *deviceEnumerator = nullptr;
    IMMDevice *device = nullptr;
    uint32_t deviceCollectionSize;

    deviceList.clear();

    //CLSID_MMDeviceEnumerator or __uuidof(IMMDeviceEnumerator)
    hr = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
    if (hr!=S_OK) throw std::runtime_error("Unable to CoCreateInstance(). Error code: " + WinUtils::HRESULTtoString(hr));

    // include capture endpoints (eCapture) to collection
    hr = deviceEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &deviceCollection);
    if (hr!=S_OK) throw std::runtime_error("Unable to deviceEnumerator->EnumAudioEndpoints(). Error code: " + WinUtils::HRESULTtoString(hr));
    hr = deviceCollection->GetCount(&deviceCollectionSize);
    if (hr!=S_OK) throw std::runtime_error("Unable to deviceCollection->GetCount(). Error code: " + WinUtils::HRESULTtoString(hr));
    for (uint32_t deviceIndex = 0; deviceIndex < deviceCollectionSize; deviceIndex++) {
        deviceCollection->Item(deviceIndex, &device);
        deviceList.emplace_back(Device(device, CASM::CAPTURE));
    }
    deviceCollection->Release();

    // include rendering endpoints (eRender) to collection
    hr = deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &deviceCollection);
    if (hr!=S_OK) throw std::runtime_error("Unable to deviceEnumerator->EnumAudioEndpoints(). Error code: " + WinUtils::HRESULTtoString(hr));
    hr = deviceCollection->GetCount(&deviceCollectionSize);
    if (hr!=S_OK) throw std::runtime_error("Unable to deviceCollection->GetCount(). Error code: " + WinUtils::HRESULTtoString(hr));
    for (uint32_t deviceIndex = 0; deviceIndex < deviceCollectionSize; deviceIndex++) {
        deviceCollection->Item(deviceIndex, &device);
        deviceList.emplace_back(Device(device, CASM::RENDER));
    }
    deviceCollection->Release();

    deviceCount = (uint_fast32_t) deviceList.size();
    deviceEnumerator->Release();
    device->Release();

    return 0;
}

}