// =============== DESCRIPTION ===============
// Created on 26 May.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_DEVICE_BASE_HPP
#define CROSSAUDIOSTREAMMANAGER_DEVICE_BASE_HPP

#include <CASM/CASM.hpp>
#include "buffer.hpp"
#include <string>

class DeviceBase {
public:
    DeviceBase(){};
    virtual ~DeviceBase() {};

    WaveProperties getBufferWaveProperties() {
        return buffer.getWaveProperties();
    };
    WaveProperties getDeviceWaveProperties() {
        return waveProperties;
    };
    std::wstring getDescpiption() {
        return description;
    };

    virtual int open(CASM::Access access, std::chrono::duration<double> fragmentDuration)=0;
    virtual int close()=0;
    virtual Buffer read()=0;
    virtual int write(Buffer data)=0;

protected:
    std::wstring name;
    std::wstring description;
    WaveProperties waveProperties;
    uint64_t fragmentDurationRequested;
    Buffer buffer;
    bool active;
    CASM::DeviceType type;
};

template <class TDeviceHandler, class TAudioStream>
class DeviceTemplate : public DeviceBase {
public:
    typedef TAudioStream AudioStream;

    DeviceTemplate();
    DeviceTemplate(TDeviceHandler *handler, CASM::DeviceType deviceType);
    virtual ~DeviceTemplate();

    virtual int open(CASM::Access access, std::chrono::duration<double> fragmentDuration)=0;
    virtual int close()=0;
    virtual Buffer read()=0;
    virtual int write(Buffer data)=0;

protected:
    TDeviceHandler *handler;
    AudioStream stream;
};

#include "device_base.inl"

#endif //CROSSAUDIOSTREAMMANAGER_DEVICE_BASE_HPP
