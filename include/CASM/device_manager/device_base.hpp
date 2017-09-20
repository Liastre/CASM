// =============== DESCRIPTION ===============
// Created on 26 May.
// ===========================================

#ifndef CASM_DEVICE_BASE_HPP
#define CASM_DEVICE_BASE_HPP

#include <CASM/CASM.hpp>
#include <CASM/core/end_point.hpp>
#include <string>


namespace CASM {

/// @class DeviceBase
/// @brief base class for Device object.
class DeviceInterface : public virtual EndPointInterface {
public:
    DeviceInterface() = default;
    ~DeviceInterface() override = default;

    virtual WaveProperties getDeviceWaveProperties()=0;
    virtual std::wstring getDescription()=0;
};

template < class TDeviceHandler >
class DeviceBase : public virtual DeviceInterface, public EndPointBase {
public:
    DeviceBase();
    DeviceBase(void *handler, CASM::DeviceType deviceType);
    ~DeviceBase() override;

    // getters
    WaveProperties getDeviceWaveProperties() final;
    std::wstring getDescription() final;

protected:
    TDeviceHandler *handler;
    std::wstring name;
    std::wstring description;
    /// @brief actual device wave properties
    WaveProperties deviceWaveProperties;
    CASM::DeviceType type;
    uint32_t bufferFramesCount;
    std::chrono::duration< double > bufferDuration;
};

}

#include "device_base.inl"


#endif //CASM_DEVICE_BASE_HPP
