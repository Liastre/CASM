namespace CASM {

template < class TDeviceHandler >
DeviceBase< TDeviceHandler >::DeviceBase() {
    _bufferDuration = std::chrono::duration<double>::zero();
    _handler = nullptr;
    _name = L"";
    _description = L"";
}


template < class TDeviceHandler >
DeviceBase< TDeviceHandler >::DeviceBase(void *deviceHandler, CASM::DeviceType deviceType) :DeviceBase() {
    _handler = static_cast<TDeviceHandler *>(deviceHandler);
    _type = deviceType;
}


template < class TDeviceHandler >
DeviceBase< TDeviceHandler >::~DeviceBase() {
}


template < class TDeviceHandler >
WaveProperties DeviceBase< TDeviceHandler >::getDeviceWaveProperties() {
    return _deviceWaveProperties;
}


template < class TDeviceHandler >
std::wstring DeviceBase< TDeviceHandler >::getDescription() {
    return _description;
}

}