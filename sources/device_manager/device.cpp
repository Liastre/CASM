#include "CASM/device.hpp"
#include "windows/device_windows_wasapi.hpp"


namespace CASM {

Device::Device() {
}


Device::Device(void *deviceHandler, CASM::DeviceType deviceType) {
    device = std::make_shared< DeviceWindowsWASAPI >(deviceHandler, deviceType);
}


Device::~Device() {
    device.reset();
};


Buffer Device::open(std::chrono::duration< double > duration) {
    return device->open(duration);
}


bool Device::open(Buffer buffer) {
    return device->open(buffer);
}


void Device::close() {
    device->close();
}


bool Device::read(Buffer &buffer) {
    return device->read(buffer);
}


bool Device::write(Buffer buffer) {
    device->write(buffer);
}


WaveProperties Device::getDeviceWaveProperties() {
    return device->getDeviceWaveProperties();
}


WaveProperties Device::getStreamWaveProperties() {
    return device->getStreamWaveProperties();
}


std::wstring Device::getDescription() {
    return device->getDescription();
}


bool Device::isAvailable() {
    return true;
}

}
