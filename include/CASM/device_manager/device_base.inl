template <class TDeviceHandler>
DeviceBase<TDeviceHandler>::DeviceBase() {
    bufferDuration = std::chrono::duration<double>::zero();
    handler = nullptr;
    name = L"";
    description = L"";
    active = false;
}

template <class TDeviceHandler>
DeviceBase<TDeviceHandler>::DeviceBase(void* deviceHandler, CASM::DeviceType deviceType) : DeviceBase(){
    handler = static_cast<TDeviceHandler*>(deviceHandler);
    type = deviceType;
}

template <class TDeviceHandler>
DeviceBase<TDeviceHandler>::~DeviceBase() {
}

template <class TDeviceHandler>
bool DeviceBase<TDeviceHandler>::init(std::chrono::duration<double> bufferDuration)
{
    if (active) {
        return false;
    }
    DeviceBase::bufferDuration = bufferDuration;

    return true;
}

template <class TDeviceHandler>
WaveProperties DeviceBase<TDeviceHandler>::getDeviceWaveProperties() {
    return deviceWaveProperties;
}

template <class TDeviceHandler>
std::wstring DeviceBase<TDeviceHandler>::getDescription() {
    return description;
}
