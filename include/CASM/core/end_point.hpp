// =============== DESCRIPTION ===============
// ===========================================

#ifndef CASM_END_POINT_HPP
#define CASM_END_POINT_HPP

#include <CASM/core/buffer.hpp>

/// @class EndPointInterface
/// @brief abstract interface class
class EndPointInterface{
public:
    EndPointInterface() = default;
    virtual ~EndPointInterface() = default;


    /// @brief read from EndPoint to Buffer
    /// @param [out] buffer - Buffer where we push data
    /// @return true if we read all data, false if some data left
    virtual bool read(Buffer& buffer)=0;
    /// @brief write to EndPoint from Buffer
    /// @param [in] buffer - Buffer where we take data
    /// @return true if we wrote all data, false if some data left
    virtual bool write(Buffer buffer)=0;
    /// @brief check if EndPoint still active and available
    /// @return true if we EndPoint still available, false if don't
    virtual bool isAvailable()=0;
};

#endif //CASM_END_POINT_HPP
