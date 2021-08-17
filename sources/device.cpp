#include "CASM/device.hpp"
#include "device_api/wasapi/device.hpp"


namespace CASM {


Device::Device(void * deviceHandler, DeviceType deviceType) {
    _device = std::make_shared<DeviceApi::Wasapi::Device>(deviceHandler, deviceType);
}


Device::Device(Device const& device) {
    this->operator=(device);
}


Device::Device(Device&& device) noexcept {
    this->operator=(std::move(device));
}


Device& Device::operator=(Device const& device) {
    _device = device._device;
    return *this;
}


Device& Device::operator=(Device&& device) {
    _device = std::move(device._device);
    return *this;
}


Device::~Device() {
    _device.reset();
}


bool Device::openCaptureStream(Duration const & duration, Buffer & buffer) {
    // TODO: check exclusive mode
    if (_device->isInUsage()) {
        throw std::logic_error("Device in exclusive use");
    }

    return _device->openCaptureStream(duration, buffer);
}


bool Device::openRenderStream(Buffer const & buffer) {
    // TODO: check exclusive mode
    if (_device->isInUsage()) {
        throw std::logic_error("Device in exclusive use");
    }

    return _device->openRenderStream(buffer);
}


void Device::closeCaptureStream() {
    _device->closeCaptureStream();
}


void Device::closeRenderStream() {
    _device->closeRenderStream();
}


BufferStatus
Device::read(Buffer& buffer) {
    return _device->read(buffer);
}


bool Device::write(Buffer const & buffer) {
    _device->write(buffer);
    return true;
}

// TODO: delete
WaveProperties Device::getDeviceWaveProperties() {
    return _device->getDeviceWaveProperties();
}

// TODO: rename to getWaveProperties
WaveProperties Device::getStreamWaveProperties() const {
    // TODO: use inbuilt getStreamWaveProperties
    return _device->getStreamWaveProperties();
}

String
Device::getDescription() {
    return _device->getDescription();
}

bool Device::isAvailable() const {
    return true;
}


bool Device::isInUsage() const {
    return _device->isInUsage();
}


bool Device::isValid() const {
    return _device->isValid();
}


Device::operator bool() const {
    return isValid();
}

} // namespace CASM
