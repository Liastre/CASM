/**
 * @brief DeviceManager WASAPI implementation
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include <CASM/device_manager/device_manager_base.hpp>

namespace CASM {
namespace DeviceApi {
namespace Wasapi {

class Enumerator final : public EnumeratorBase {
public:
    Enumerator();
    Enumerator(Enumerator const&);
    Enumerator(Enumerator&&) noexcept;
    Enumerator& operator=(Enumerator const&);
    Enumerator& operator=(Enumerator&&) noexcept;
    ~Enumerator();

    int update() final;
};

} // namespace Wasapi
} // namespace Device
} // namespace CASM
