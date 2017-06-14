#include <CASM/CASM.hpp>
#include <iostream>
#include "sources/device_manager_windows_wasapi.hpp"
#include "sources/file.hpp"

int main()
{
    /*/ init Device Manager
    DeviceManagerWindowsWASAPI deviceManager;
    deviceManager.update();

    // choose Device
    int deviceCount = deviceManager.getDeviceCount();
    for(unsigned int i=0; i<deviceCount; i++) {
        std::wcout << i << ": " << deviceManager.getDevice(i).getDescpiption() << std::endl;
    }
    unsigned int deviceIndex;
    std::cin >> deviceIndex;
    DeviceWindowsWASAPI endPoint = deviceManager.getDevice(deviceIndex);

    // init device and file
    endPoint.open(CASM::READ, std::chrono::seconds(1));
    File file("endPointDevice.wav", CASM::WRITE, endPoint.getBufferWaveProperties());

    // write data
    for (int i=0; i<16; i++) {
        file.write(endPoint.read());
    }
    file.finalize();
    endPoint.close();*/

    // init Device Manager
    DeviceManagerWindowsWASAPI deviceManager;
    deviceManager.update();

    // choose Device
    int deviceCount = deviceManager.getDeviceCount();
    for(unsigned int i=0; i<deviceCount; i++) {
        std::wcout << i << ": " << deviceManager.getDevice(i).getDescpiption() << std::endl;
    }
    unsigned int deviceIndex;
    std::cin >> deviceIndex;
    DeviceWindowsWASAPI *test = new DeviceWindowsWASAPI;
    DeviceWindowsWASAPI test2 = deviceManager.getDevice(deviceIndex);
    *test = test2;

    try {
        // init device and file
        test->open(CASM::READ, std::chrono::seconds(1));
    } catch (const std::exception &e) {
        std::cout << e.what() << '\n';
    }
    File file("endPointDevice.wav", CASM::WRITE, test->getBufferWaveProperties());

    // write data
    for (int i=0; i<16; i++) {
        file.write(test->read());
    }
    file.finalize();
    test->close();

    return 0;
}