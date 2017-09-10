#include "CASM/device.hpp"
#include "windows/device_windows_wasapi.hpp"


Device::Device() {
}

Device::Device(void* deviceHandler, CASM::DeviceType deviceType){
device = std::make_shared<DeviceWindowsWASAPI>(deviceHandler, deviceType);
}

Device::~Device() {
};

void Device::init(std::chrono::duration<double> bufferDuration) {
    device->init(bufferDuration);
}

void Device::open(CASM::Access access) {
    device->open(access);
}

void Device::close() {
    device->close();
}

bool Device::read(Buffer& buffer) {
    return device->read(buffer);
}

bool Device::write(Buffer buffer) {
    device->write(buffer);
}

WaveProperties Device::getDeviceWaveProperties()
{
    return device->getDeviceWaveProperties();
}

std::wstring Device::getDescription() {
    return device->getDescription();
}

bool Device::isAvailable() {
    return true;
}

