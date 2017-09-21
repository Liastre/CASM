#include <CASM/CASM.hpp>
#include <CASM/device_manager.hpp>
#include <CASM/file.hpp>
#include <iostream>
#include "../sources/stream.hpp"


int main() {
    // choose device
    CASM::DeviceManager deviceManager;
    deviceManager.update();
    int deviceCount = deviceManager.getDeviceCount();
    for (unsigned int i = 0; i < deviceCount; i++) {
        std::wcout << i << ": " << deviceManager.getDevice(i).getDescription() << std::endl;
    }
    unsigned int deviceIndex;
    std::cin >> deviceIndex;

    // init endpoints
    CASM::Device endPoint = deviceManager.getDevice(deviceIndex);
    CASM::File file("endPointDevice.wav");

    CASM::Stream connector1(endPoint, file);
    connector1.start();
    // request audio stream waiting for 5 seconds before closing
    connector1.stop(std::chrono::seconds(5));
    // waiting for audio stream finished
    connector1.join();

    CASM::Stream connector2(file, endPoint);
    connector2.start();
    // we are not forcing stop, just waiting for end of file
    connector2.join();

    return 0;
}
