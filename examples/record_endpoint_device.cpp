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

    endPoint.open(CASM::READ, std::chrono::seconds(1));
    File file("endPointDevice.wav", CASM::WRITE, endPoint.getStreamWaveProperties());

    // write data
    for (int i=0; i<8; i++) {
        file.write(endPoint.read());
    }
    file.finalize();
    endPoint.close();

    return 0;
}