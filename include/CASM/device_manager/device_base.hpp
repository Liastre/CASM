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
class DeviceBase : public EndPointInterface {
public:
    DeviceBase();
    ~DeviceBase() override = default;

    virtual void init(std::chrono::duration<double> bufferDuration)=0;
    WaveProperties getDeviceWaveProperties();
    WaveProperties getStreamWaveProperties();
    std::wstring getDescpiption();

protected:
    std::wstring name;
    std::wstring description;
    WaveProperties deviceWaveProperties;    ///< actual device wave properties
    WaveProperties streamWaveProperties;    ///< output stream properties
    bool active;
    CASM::DeviceType type;
};

template <class TDeviceHandler>
class DeviceTemplate : public DeviceBase {
public:
    DeviceTemplate();
    DeviceTemplate(void* handler, CASM::DeviceType deviceType);
    ~DeviceTemplate() override;

    void init(std::chrono::duration<double> bufferDuration) final;

protected:
    TDeviceHandler *handler;
    std::chrono::duration<double> bufferDuration;
};

#include "device_base.inl"

#endif //CASM_DEVICE_BASE_HPP
