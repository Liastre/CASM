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

class EnumeratorInterface {
public:
    virtual ~EnumeratorInterface() = default;

    virtual bool update(std::vector<Device>& deviceList) = 0;
};

} // namespace DeviceApi
} // namespace CASM
