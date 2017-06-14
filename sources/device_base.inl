template <class TDeviceHandler, class TAudioStream>
DeviceTemplate<TDeviceHandler, TAudioStream>::DeviceTemplate() {
    handler = nullptr;
    stream = nullptr;
    active = false;
}

template <class TDeviceHandler, class TAudioStream>
DeviceTemplate<TDeviceHandler, TAudioStream>::DeviceTemplate(TDeviceHandler *deviceHandler, CASM::DeviceType deviceType) : DeviceTemplate(){
    handler = deviceHandler;
    type = deviceType;
}

template <class TDeviceHandler, class TAudioStream>
DeviceTemplate<TDeviceHandler, TAudioStream>::~DeviceTemplate() {
}
