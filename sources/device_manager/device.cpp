#include "CASM/device.hpp"
#include "windows/device_windows_wasapi.hpp"


Device::Device(){
}

Device::Device(void* deviceHandler, CASM::DeviceType deviceType){
device = std::make_shared<DeviceWindowsWASAPI>(deviceHandler, deviceType);
}

Device::~Device(){
};

int Device::open(CASM::Access access, std::chrono::duration<double> fragmentDuration) {
    return device->open(access, fragmentDuration);
}

int Device::close() {
    return device->close();
}

Buffer Device::read() {
    return device->read();
}

int Device::write(Buffer data){
    return device->write(data);
}

WaveProperties Device::getBufferWaveProperties(){
    return device->getBufferWaveProperties();
}

std::wstring Device::getDescpiption() {
    return device->getDescpiption();
}
