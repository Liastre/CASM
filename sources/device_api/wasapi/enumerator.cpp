#include "CASM/device_api/wasapi/enumerator.hpp"
#include "windows_wasapi.hpp"
#include <cassert>

namespace {

std::atomic<std::size_t> kIsInitialized = { 0 };

HRESULT
comInitialize() {
    HRESULT hr{ S_OK };
    if (kIsInitialized == 0) {
        hr = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    }
    ++kIsInitialized;

    return hr;
}

void
comUninitialize() {
    --kIsInitialized;
    if (kIsInitialized == 0) {
        ::CoUninitialize();
    }
}

} // namespace

namespace CASM {
namespace DeviceApi {
namespace Wasapi {

Enumerator::Enumerator() {
    HRESULT hr = comInitialize();
    if (hr != S_OK) {
        throw std::runtime_error("Unable to CoInitializeEx(). Error code: " + WinUtils::HRESULTtoString(hr));
    }
}

Enumerator::Enumerator(Enumerator const& enumerator) {
    operator=(enumerator);
}

Enumerator::Enumerator(Enumerator&& enumerator) noexcept {
    operator=(std::move(enumerator));
}

Enumerator&
Enumerator::operator=(Enumerator&& enumerator) noexcept {
    comInitialize();
    return *this;
}

Enumerator&
Enumerator::operator=(Enumerator const& enumerator) {
    comInitialize();
    return *this;
}

Enumerator::~Enumerator() {
    comUninitialize();
}

int
Enumerator::update() {
    HRESULT hr;

    IMMDeviceCollection* deviceCollection = nullptr;
    IMMDeviceEnumerator* deviceEnumerator = nullptr;
    IMMDevice* device = nullptr;
    uint32_t deviceCollectionSize;

    _deviceList.clear();

    //CLSID_MMDeviceEnumerator or __uuidof(IMMDeviceEnumerator)
    hr = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
    if (hr != S_OK)
        throw std::runtime_error("Unable to CoCreateInstance(). Error code: " + WinUtils::HRESULTtoString(hr));

    // include capture endpoints (eCapture) to collection
    hr = deviceEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &deviceCollection);
    if (hr != S_OK)
        throw std::runtime_error("Unable to deviceEnumerator->EnumAudioEndpoints(). Error code: " + WinUtils::HRESULTtoString(hr));
    hr = deviceCollection->GetCount(&deviceCollectionSize);
    if (hr != S_OK)
        throw std::runtime_error("Unable to deviceCollection->GetCount(). Error code: " + WinUtils::HRESULTtoString(hr));
    for (uint32_t deviceIndex = 0; deviceIndex < deviceCollectionSize; deviceIndex++) {
        deviceCollection->Item(deviceIndex, &device);
        _deviceList.emplace_back(Device(device, DeviceType::CAPTURE));
    }
    deviceCollection->Release();

    // include rendering endpoints (eRender) to collection
    hr = deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &deviceCollection);
    if (hr != S_OK)
        throw std::runtime_error("Unable to deviceEnumerator->EnumAudioEndpoints(). Error code: " + WinUtils::HRESULTtoString(hr));
    hr = deviceCollection->GetCount(&deviceCollectionSize);
    if (hr != S_OK)
        throw std::runtime_error("Unable to deviceCollection->GetCount(). Error code: " + WinUtils::HRESULTtoString(hr));
    for (uint32_t deviceIndex = 0; deviceIndex < deviceCollectionSize; deviceIndex++) {
        deviceCollection->Item(deviceIndex, &device);
        _deviceList.emplace_back(Device(device, DeviceType::RENDER));
    }
    deviceCollection->Release();

    _deviceCount = _deviceList.size();
    deviceEnumerator->Release();

    return 0;
}

} // namespace Wasapi
} // namespace Device
} // namespace CASM
