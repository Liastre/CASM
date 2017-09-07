#include "CASM/device.hpp"
#include "windows/device_windows_wasapi.hpp"


Device::Device() {
}

Device::Device(void* deviceHandler, CASM::DeviceType deviceType){
device = std::make_shared<DeviceWindowsWASAPI>(deviceHandler, deviceType);
}

Device::~Device() {
};

int Device::open(CASM::Access access, std::chrono::duration<double> bufferDuration) {
    return device->open(access, bufferDuration);
}

int Device::close() {
    return device->close();
}

bool Device::read(Buffer& buffer) {
    return device->read(buffer);
}

bool Device::write(Buffer buffer) {
    device->write(buffer);
}

WaveProperties Device::getStreamWaveProperties() {
    return device->getStreamWaveProperties();
}

WaveProperties Device::getDeviceWaveProperties()
{
    return device->getDeviceWaveProperties();
}

std::wstring Device::getDescpiption() {
    return device->getDescpiption();
}

bool Device::isAvailable() {
    return true;
}

