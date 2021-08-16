#include "CASM/device_api/enumerator_base.hpp"

namespace CASM {
namespace DeviceApi {

Device&
EnumeratorBase::getDevice(std::size_t index) {
    return _deviceList[index];
};

std::size_t
EnumeratorBase::getDeviceCount() {
    return _deviceCount;
}

} // namespace DeviceApi
} // namespace CASM
