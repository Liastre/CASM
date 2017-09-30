/**
    @file device.cpp
    @copyright LGPLv3
    @brief definition of Device class
**/

#include <CASM/device.hpp>
#include "windows/device_windows_wasapi.hpp"


namespace CASM {

Device::Device() {
}


Device::Device(void *deviceHandler, CASM::DeviceType deviceType) {
    device = std::make_shared< DeviceWindowsWASAPI >(deviceHandler, deviceType);
}


Device::~Device() {
    device.reset();
}


Buffer Device::openCaptureStream(std::chrono::duration< double > duration) {
    //if (device->isInUsage()) throw std::logic_error("Device already in use");

    return device->openCaptureStream(duration);
}


bool Device::openRenderStream(Buffer buffer) {
    if (device->isInUsage()) throw std::logic_error("Device already in use");

    return device->openRenderStream(buffer);
}


void Device::closeCaptureStream() {
    device->closeCaptureStream();
}

void Device::closeRenderStream() {
    device->closeRenderStream();
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


bool Device::isInUsage() {
    return device->isInUsage();
}

} // namespace CASM
