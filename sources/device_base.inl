template <class TDeviceHandler, class TAudioStream>
DeviceTemplate<TDeviceHandler, TAudioStream>::DeviceTemplate() {
    handler = nullptr;
    stream = nullptr;
    active = false;
}

template <class TDeviceHandler, class TAudioStream>
DeviceTemplate<TDeviceHandler, TAudioStream>::DeviceTemplate(DeviceHandler *deviceHandler, CASM::DeviceType deviceType) : DeviceTemplate(){
    handler = deviceHandler;
    type = deviceType;
}

template <class TDeviceHandler, class TAudioStream>
DeviceTemplate<TDeviceHandler, TAudioStream>::~DeviceTemplate() {
}

template <class TDeviceHandler, class TAudioStream>
std::wstring DeviceTemplate<TDeviceHandler, TAudioStream>::getDescpiption() {
    return description;
}
