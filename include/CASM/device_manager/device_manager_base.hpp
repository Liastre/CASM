/**
 * Base class for DeviceManager
 */

#pragma once

#include <CASM/device.hpp>
#include "CASM/core/wave_properties.hpp"
#include <vector>
#include <string>

namespace CASM {
namespace DeviceApi {

class EnumeratorBase {
public:
    EnumeratorBase() = default;
    EnumeratorBase(EnumeratorBase const&) = default;
    EnumeratorBase(EnumeratorBase&&) = default;
    EnumeratorBase& operator=(EnumeratorBase const&) = default;
    EnumeratorBase& operator=(EnumeratorBase&&) = default;
    virtual ~EnumeratorBase() = default;

    Device& getDevice(std::size_t index);
    std::size_t getDeviceCount();

    virtual int update() = 0;

protected:
    std::vector<Device> _deviceList;
    std::size_t _deviceCount = { 0 };
};

} // namespace Device
} // namespace CASM
