#include <CASM/CASM.hpp>
#include <CASM/device_manager.hpp>
#include <CASM/file.hpp>
#include <iostream>


int main()
{
    // init Device Manager
    DeviceManager deviceManager;
    deviceManager.update();

    // choose Device
    int deviceCount = deviceManager.getDeviceCount();
    for(unsigned int i=0; i<deviceCount; i++) {
        std::wcout << i << ": " << deviceManager.getDevice(i).getDescpiption() << std::endl;
    }
    unsigned int deviceIndex;
    std::cin >> deviceIndex;
    Device endPoint = deviceManager.getDevice(deviceIndex);

    endPoint.init(std::chrono::seconds(1));
    endPoint.open(CASM::READ);

    File file("endPointDevice.wav", endPoint.getStreamWaveProperties());

    // write data
    Buffer buffer(endPoint.getStreamWaveProperties(), std::chrono::seconds(1));
    for (int i=0; i<8; i++) {
        endPoint.read(buffer);
        file.write(buffer);
    }
    file.close();
    endPoint.close();

    return 0;
}
