#include <CASM/CASM.hpp>
#include <CASM/device_manager.hpp>
#include <CASM/file.hpp>
#include <iostream>
#include <thread>


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

    // open endpoints
    CASM::Buffer buffer = endPoint.open(std::chrono::seconds(1));
    file.open(buffer);

    // write data
    for (int i = 0; i < 8; i++) {
        std::this_thread::sleep_for(buffer.getDuration());
        endPoint.read(buffer);
        file.write(buffer);
    }

    // close endpoints
    file.close();
    endPoint.close();

    return 0;
}
