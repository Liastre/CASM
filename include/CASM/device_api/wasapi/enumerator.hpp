/**
 * @brief DeviceManager WASAPI implementation
 * @author Liastre
 * @copyright MIT
 */

#pragma once

#include "../enumerator_interface.hpp"

namespace CASM {
namespace DeviceApi {
namespace Wasapi {

class Enumerator final : public EnumeratorInterface {
public:
    Enumerator();
    Enumerator(Enumerator const&);
    Enumerator(Enumerator&&) noexcept;
    Enumerator& operator=(Enumerator const&);
    Enumerator& operator=(Enumerator&&) noexcept;
    ~Enumerator();

    bool update(std::vector<CASM::Device>& deviceList) final;
};

} // namespace Wasapi
} // namespace Device
} // namespace CASM
