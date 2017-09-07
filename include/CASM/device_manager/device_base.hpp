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
class DeviceBase : public EndPointInterface {
public:
    DeviceBase() = default;
    virtual ~DeviceBase() = default;

    WaveProperties getStreamWaveProperties() {
        return  streamWaveProperties;
    }
    WaveProperties getDeviceWaveProperties() {
        return deviceWaveProperties;
    }
    std::wstring getDescpiption() {
        return description;
    }

    virtual int open(CASM::Access access, std::chrono::duration<double> fragmentDuration)=0;
    virtual int close()=0;

protected:
    std::wstring name;
    std::wstring description;
    WaveProperties deviceWaveProperties;    ///< actual device wave properties
    WaveProperties streamWaveProperties;    ///< output stream properties
    uint64_t fragmentDurationRequested;
    bool active;
    CASM::DeviceType type;
};

template <class TDeviceHandler>
class DeviceTemplate : public DeviceBase {
public:
    DeviceTemplate();
    DeviceTemplate(void* handler, CASM::DeviceType deviceType);
    ~DeviceTemplate() override;

    int open(CASM::Access access, std::chrono::duration<double> fragmentDuration) override =0;
    int close() override =0;

protected:
    TDeviceHandler *handler;
};

#include "device_base.inl"

#endif //CASM_DEVICE_BASE_HPP
