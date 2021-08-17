/**
 * Declaration for EndPointInterface and EndPointBase classes
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include <CASM/types.hpp>
#include <CASM/core/buffer.hpp>

namespace CASM {

// TODO: enum for states
enum EndPointState {
    ACTIVE, // device is open and in usage and receiving data
    IDLE, // device is open and ready for receive data
    INACTIVE // device closed
};

/// @class EndPointInterface
/// @brief EndPoint abstract interface class
class EndPointInterface {
public:
    EndPointInterface() = default;
    virtual ~EndPointInterface() = default;

    /// @brief set EndPoint ready for transferring data and initialize the buffer
    /// @param [in] duration
    /// @param [out] buffer
    /// @return true if success and false if impossible
    virtual bool openCaptureStream(Duration const& duration, Buffer& buffer) = 0;
    /// @brief set EndPoint ready for receiving data and initialize the buffer
    /// @param [in] buffer
    /// @return true if success and false if impossible
    virtual bool openRenderStream(Buffer const& buffer) = 0;
    /// @brief close EndPoint capture stream
    virtual void closeCaptureStream() = 0;
    /// @brief close EndPoint record stream
    virtual void closeRenderStream() = 0;
    /**
     * Read from EndPoint to Buffer
     * @param[out] buffer - Buffer where we push data
     * @return true if we read all data, false if some data left
     */
    virtual BufferStatus read(Buffer& buffer) = 0;
    /// @brief write to EndPoint from Buffer
    /// @param [in] buffer - Buffer where we take data
    /// @return true if we wrote all data, false if some data left
    virtual bool write(Buffer const& buffer) = 0;

    /// @brief get output stream wave properties
    /// @return output stream wave properties
    virtual WaveProperties getStreamWaveProperties() const = 0;
    /// @brief check if EndPoint still active and available
    /// @return true if we EndPoint still available, false if don't
    virtual bool isAvailable() const = 0;
    /// @brief check if EndPoint in usage
    /// @return true if we EndPoint still available, false if don't
    virtual bool isInUsage() const = 0;
    /// @brief check if EndPoint is valid object
    /// @return true if valid, false if not
    virtual bool isValid() const = 0;
};

/// @brief EndPoint base class
class EndPointBase : public virtual EndPointInterface {
public:
    EndPointBase() = default;
    EndPointBase(EndPointBase const& endpoint) {
        this->operator=(endpoint);
    }
    EndPointBase(EndPointBase&& endpoint) noexcept {
        this->operator=(std::move(endpoint));
    }
    EndPointBase& operator=(EndPointBase const& endpoint) {
        _isValid = endpoint._isValid.load();
        _active = endpoint._active.load();
        _streamWaveProperties = endpoint._streamWaveProperties;
        return *this;
    }

    EndPointBase& operator=(EndPointBase&& endpoint) noexcept {
        _isValid = endpoint._isValid.load();
        _active = endpoint._active.load();
        _streamWaveProperties = std::move(endpoint._streamWaveProperties);
        return *this;
    }

    virtual ~EndPointBase() = default;

    WaveProperties getStreamWaveProperties() const final {
        return _streamWaveProperties;
    }

    bool isInUsage() const final {
        return _active;
    }

    bool isValid() const final {
        return _isValid;
    }

    operator bool() const {
        return isValid();
    }

protected:
    std::atomic<bool> _isValid = { false };
    std::atomic<bool> _active = { false };
    WaveProperties _streamWaveProperties = {};
};

} // namespace CASM
