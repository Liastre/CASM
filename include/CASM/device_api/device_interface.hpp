/**
 * Declaration for DeviceInterface and DeviceBase classes
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include <CASM/types.hpp>
#include <CASM/core/end_point.hpp>
#include <string>

namespace CASM {
namespace DeviceApi {

struct StreamProperties {
    FramesCount framesCount = { 0 };
    WaveProperties waveProperties = { };
};

/**
 * @brief Interface class for Device object
 */
class DeviceInterface {
public:
    DeviceInterface() = default;
    virtual ~DeviceInterface() = default;

    virtual WaveProperties const& getDeviceWaveProperties() const = 0;
    virtual String const& getName() const = 0;
    virtual String const& getDescription() const = 0;

    virtual StreamProperties openRead(Duration const& requestedDuration, bool isExclusive) = 0;
    virtual StreamProperties openWrite(Duration const& requestedDuration, bool isExclusive) = 0;
    virtual void closeRead() = 0;
    virtual void closeWrite() = 0;
    virtual BufferStatus readData(Buffer& buffer, std::uint16_t blockAlign) = 0;
    virtual bool writeData(Buffer const& buffer, std::uint16_t blockAlign) = 0;
};

/**
 * @brief Base class for Device object.
 */
class DeviceBase : public virtual DeviceInterface {
public:
    DeviceBase() = default;
    DeviceBase(CASM::DeviceType deviceType) {
        _type = deviceType;
    }
    virtual ~DeviceBase() = default;

    WaveProperties const& getDeviceWaveProperties() const final {
        return _deviceWaveProperties;
    }

    DeviceType getDeviceType() {
        return _type;
    }

protected:
    /// @brief actual device wave properties
    WaveProperties _deviceWaveProperties;

private:
    DeviceType _type = DeviceType::Undefined;
};

} // namespace DeviceApi
} // namespace CASM
