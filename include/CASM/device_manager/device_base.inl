template <class TDeviceHandler>
DeviceTemplate<TDeviceHandler>::DeviceTemplate() {
    handler = nullptr;
    active = false;
}

template <class TDeviceHandler>
DeviceTemplate<TDeviceHandler>::DeviceTemplate(void* deviceHandler, CASM::DeviceType deviceType) : DeviceTemplate(){
    handler = static_cast<TDeviceHandler*>(deviceHandler);
    type = deviceType;
}

template <class TDeviceHandler>
DeviceTemplate<TDeviceHandler>::~DeviceTemplate() {
}
