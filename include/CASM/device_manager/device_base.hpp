// =============== DESCRIPTION ===============
// Created on 26 May.
// ===========================================

#ifndef CASM_DEVICE_BASE_HPP
#define CASM_DEVICE_BASE_HPP

#include <CASM/CASM.hpp>
#include <CASM/core/end_point.hpp>
#include <string>

/// @class DeviceBase
/// @brief base class for Device object.
// TODO: DeviceBase to DeviceInterface, pure abstract class and DeviceTemplate to DeviceBase
// TODO: remove getStreamWaveProperties() and possible rename to gerEndPointWaveProperties?
class DeviceInterface : public EndPointBase {
public:
    DeviceInterface() = default;
    ~DeviceInterface() override = default;

    virtual void init(std::chrono::duration<double> bufferDuration)=0;
    virtual WaveProperties getDeviceWaveProperties()=0;
    virtual std::wstring getDescription()=0;
};

template <class TDeviceHandler>
class DeviceBase : public DeviceInterface {
public:
    DeviceBase();
    DeviceBase(void* handler, CASM::DeviceType deviceType);
    ~DeviceBase() override;

    void init(std::chrono::duration<double> bufferDuration) final;
    WaveProperties getDeviceWaveProperties() final;
    std::wstring getDescription() final;

protected:
    TDeviceHandler *handler;
    std::wstring name;
    std::wstring description;
    WaveProperties deviceWaveProperties;    ///< actual device wave properties
    WaveProperties streamWaveProperties;    ///< output stream properties
    bool active;
    CASM::DeviceType type;
    std::chrono::duration<double> bufferDuration;
};

#include "device_base.inl"

#endif //CASM_DEVICE_BASE_HPP
