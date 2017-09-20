// =============== DESCRIPTION ===============
// ===========================================

#ifndef CASM_END_POINT_HPP
#define CASM_END_POINT_HPP

#include <CASM/CASM.hpp>
#include <CASM/core/buffer.hpp>

/// @class EndPointInterface
/// @brief EndPoint abstract interface class
class EndPointInterface {
public:
    EndPointInterface() = default;
    virtual ~EndPointInterface() = default;

    /// @brief set EndPoint ready for transferring data and initialize the buffer
    /// @param [in] duration
    /// @return Buffer
    virtual Buffer open(std::chrono::duration<double> duration)=0;
    /// @brief set EndPoint ready for receiving data and initialize the buffer
    /// @param [in] buffer
    /// @return true if success and false if impossible
    virtual bool open(Buffer buffer)=0;
    /// @brief close EndPoint and destroy the buffer
    virtual void close()=0;
    /// @brief read from EndPoint to Buffer
    /// @param [out] buffer - Buffer where we push data
    /// @return true if we read all data, false if some data left
    virtual bool read(Buffer& buffer)=0;
    /// @brief write to EndPoint from Buffer
    /// @param [in] buffer - Buffer where we take data
    /// @return true if we wrote all data, false if some data left
    virtual bool write(Buffer buffer)=0;

    /// @brief get output stream wave properties
    /// @return output stream wave properties
    virtual WaveProperties getStreamWaveProperties()=0;
    /// @brief check if EndPoint still active and available
    /// @return true if we EndPoint still available, false if don't
    virtual bool isAvailable()=0;
};

/// @brief EndPoint base class
class EndPointBase : public virtual EndPointInterface {
public:
    EndPointBase() {
        active = false;
        streamWaveProperties = WaveProperties();
    };
    ~EndPointBase() override = default;

    WaveProperties getStreamWaveProperties() final {
        return streamWaveProperties;
    }

protected:
    bool active;
    WaveProperties streamWaveProperties;
};

#endif //CASM_END_POINT_HPP
