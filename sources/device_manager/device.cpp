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


Device::Device(void * deviceHandler, DeviceType deviceType) {
    device = std::make_shared< DeviceWindowsWASAPI >(deviceHandler, deviceType);
}


Device::~Device() {
    device.reset();
}


bool Device::openCaptureStream(Duration const & duration, Buffer & buffer) {
    //if (device->isInUsage()) throw std::logic_error("Device already in use");

    return device->openCaptureStream(duration, buffer);
}


bool Device::openRenderStream(Buffer const & buffer) {
    if (device->isInUsage()) throw std::logic_error("Device already in use");

    return device->openRenderStream(buffer);
}


void Device::closeCaptureStream() {
    device->closeCaptureStream();
}


void Device::closeRenderStream() {
    device->closeRenderStream();
}


bool Device::read(Buffer & buffer) {
    return device->read(buffer);
}


bool Device::write(Buffer const & buffer) {
    device->write(buffer);
}


WaveProperties Device::getDeviceWaveProperties() {
    return device->getDeviceWaveProperties();
}


WaveProperties Device::getStreamWaveProperties() const {
    return device->getStreamWaveProperties();
}


std::wstring Device::getDescription() {
    return device->getDescription();
}


bool Device::isAvailable() const {
    return true;
}


bool Device::isInUsage() const {
    return device->isInUsage();
}


bool Device::isValid() const {
    return device->isValid();
}


Device::operator bool() const {
    return isValid();
}

} // namespace CASM
