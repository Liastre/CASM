/**
    @file device_base.hpp
    @copyright LGPLv3
    @brief declaration for DeviceInterface and DeviceBase classes
**/

#pragma once

#include <CASM/CASM.hpp>
#include <CASM/core/end_point.hpp>
#include <string>

namespace CASM {

/// @class DeviceInterface
/// @brief interface class for Device object
class DeviceInterface : public virtual EndPointInterface {
public:
    DeviceInterface() = default;
    ~DeviceInterface() override = default;

    virtual WaveProperties getDeviceWaveProperties() = 0;
    virtual std::wstring getDescription() = 0;
};

// TODO: add << overload with description

/// @class DeviceBase
/// @brief base class for Device object.
template <class TDeviceHandler>
class DeviceBase : public virtual DeviceInterface, public EndPointBase {
public:
    DeviceBase();
    DeviceBase(void* handler, CASM::DeviceType deviceType);
    ~DeviceBase() override;

    // getters
    WaveProperties getDeviceWaveProperties() final;
    std::wstring getDescription() final;

protected:
    TDeviceHandler* _handler;
    std::wstring _name;
    std::wstring _description;
    /// @brief actual device wave properties
    WaveProperties _deviceWaveProperties;
    CASM::DeviceType _type;
    uint32_t _bufferFramesCount;
    std::chrono::duration<double> _bufferDuration;
};

} // namespace CASM

#include "device_base.inl"
