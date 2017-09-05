// =============== DESCRIPTION ===============
// Created on 26 May.
// ===========================================

#ifndef CASM_DEVICE_BASE_HPP
#define CASM_DEVICE_BASE_HPP

#include <CASM/CASM.hpp>
#include <CASM/core/buffer.hpp>
#include <string>


/// DeviceBase - base class for Device object.
class DeviceBase {
public:
    DeviceBase() = default;
    virtual ~DeviceBase() = default;

    WaveProperties getBufferWaveProperties() {
        return buffer.getWaveProperties();
    }
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
    virtual Buffer read()=0;
    virtual int write(Buffer data)=0;

protected:
    std::wstring name;
    std::wstring description;
    WaveProperties deviceWaveProperties;    ///< actual device wave properties
    WaveProperties streamWaveProperties;    ///< output stream properties
    uint64_t fragmentDurationRequested;
    Buffer buffer;
    bool active;
    CASM::DeviceType type;
};

template <class TDeviceHandler>
class DeviceTemplate : public DeviceBase {
public:
    DeviceTemplate();
    DeviceTemplate(void* handler, CASM::DeviceType deviceType);
    virtual ~DeviceTemplate();

    virtual int open(CASM::Access access, std::chrono::duration<double> fragmentDuration)=0;
    virtual int close()=0;
    virtual Buffer read()=0;
    virtual int write(Buffer data)=0;

protected:
    TDeviceHandler *handler;
};

#include "device_base.inl"

#endif //CASM_DEVICE_BASE_HPP
