inline DeviceBase::DeviceBase() {
    name = L"";
    description = L"";
    active = false;
}

inline WaveProperties DeviceBase::getStreamWaveProperties() {
    return  streamWaveProperties;
}

inline WaveProperties DeviceBase::getDeviceWaveProperties() {
    return deviceWaveProperties;
}

inline std::wstring DeviceBase::getDescpiption() {
    return description;
}

template <class TDeviceHandler>
DeviceTemplate<TDeviceHandler>::DeviceTemplate() {
    bufferDuration = std::chrono::duration<double>::zero();
    handler = nullptr;
}

template <class TDeviceHandler>
DeviceTemplate<TDeviceHandler>::DeviceTemplate(void* deviceHandler, CASM::DeviceType deviceType) : DeviceTemplate(){
    handler = static_cast<TDeviceHandler*>(deviceHandler);
    type = deviceType;
}

template <class TDeviceHandler>
DeviceTemplate<TDeviceHandler>::~DeviceTemplate() {
}

template <class TDeviceHandler>
void DeviceTemplate<TDeviceHandler>::init(std::chrono::duration<double> bufferDuration)
{
    DeviceTemplate::bufferDuration = bufferDuration;
}
