#include "CASM/device.hpp"
#include "device_api/wasapi/device.hpp"


namespace CASM {

Device::Device(Device const& device) {
    this->operator=(device);
}


Device::Device(Device&& device) noexcept {
    this->operator=(std::move(device));
}


Device& Device::operator=(Device const& device) {
    _isValid = device._isValid.load();
    _device = device._device;
    return *this;
}


Device& Device::operator=(Device&& device) noexcept {
    _isValid = device._isValid.load();
    _device = std::move(device._device);
    return *this;
}


Device::~Device() {
    _device.reset();
}

bool
Device::open(Access access) {
    _access = access;
    switch (access) {
    case Access::READ:
    case Access::READ_EXCLUSIVE:
        break;
    case Access::WRITE:
    case Access::WRITE_EXCLUSIVE:
        break;
    }

    return true;
}

bool Device::openCaptureStream(Duration const & duration, Buffer & buffer) {
    // TODO: check exclusive mode
    if (duration == Duration::zero()) {
        throw std::logic_error("Buffer duration is zero");
    }
    if (isInUsage()) {
        throw std::logic_error("Device in exclusive use");
    }

    auto streamProperties = _device->openRead(buffer.getDuration(), false);
    _streamWaveProperties = std::move(streamProperties.waveProperties);
    buffer = Buffer(_streamWaveProperties, duration);

    return true;
}


bool Device::openRenderStream(Buffer const & buffer) {
    if (buffer.getDuration() == Duration::zero()) {
        throw std::logic_error("Buffer duration is zero");
    }
    // TODO: check exclusive mode
    if (isInUsage()) {
        throw std::logic_error("Device in exclusive use");
    }

    auto streamProperties = _device->openWrite(buffer.getDuration(), false);
    _streamWaveProperties = std::move(streamProperties.waveProperties);
    return true;
}


void Device::closeCaptureStream() {
    _device->closeRead();
}


void Device::closeRenderStream() {
    _device->closeWrite();
}


BufferStatus
Device::read(Buffer& buffer) {
    return _device->readData(buffer, _streamWaveProperties.getBlockAlign());
}


bool Device::write(Buffer const& buffer) {
    _device->writeData(buffer, _streamWaveProperties.getBlockAlign());
    return true;
}

// TODO: delete
WaveProperties Device::getDeviceWaveProperties() {
    return _device->getDeviceWaveProperties();
}

String const&
Device::getDescription() const {
    return _device->getDescription();
}

String const&
Device::getName() const {
    return _device->getName();
}

bool Device::isAvailable() const {
    return true;
}

} // namespace CASM
