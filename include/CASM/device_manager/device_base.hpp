/**
 * Declaration for DeviceInterface and DeviceBase classes
 * @author Liastre
 * @copyright MIT
 */

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
    virtual String getDescription() = 0;
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
    String getDescription() final;

protected:
    TDeviceHandler* _handler;
    String _name;
    String _description;
    /// @brief actual device wave properties
    WaveProperties _deviceWaveProperties;
    CASM::DeviceType _type;
    std::uint32_t _bufferFramesCount;
    std::chrono::duration<double> _bufferDuration;
};

} // namespace CASM

#include "device_base.inl"
