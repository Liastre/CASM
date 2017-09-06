// =============== DESCRIPTION ===============
// ===========================================

#ifndef CASM_END_POINT_HPP
#define CASM_END_POINT_HPP

#include <CASM/core/buffer.hpp>

/// EndPoint - abstract interface class
class EndPoint{
public:
    EndPoint() = default;
    virtual ~EndPoint() = default;
    virtual void write(Buffer data)=0;
    virtual Buffer read()=0;
    virtual bool isAvailable()=0;
};

#endif //CASM_END_POINT_HPP
