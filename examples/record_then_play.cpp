#include <CASM/CASM.hpp>
#include <CASM/device_manager.hpp>
#include <CASM/file.hpp>
#include <CASM/stream.hpp>
#include <iostream>
#include <io.h>
#include <fcntl.h>

int
main() {
    try {
        // TODO temp solution for cmd
        _setmode(_fileno(stdout), _O_U16TEXT);

        // choose device
        CASM::DeviceManager deviceManager;
        deviceManager.update();
        int deviceCount = deviceManager.getDeviceCount();
        for (unsigned int i = 0; i < deviceCount; i++) {
            std::wcout << i << ": " << deviceManager.getDevice(i).getDescription() << std::endl;
        }

        // get device index
        unsigned int deviceIndex;
        std::wcout << L"Choose device index ...";
        std::cin >> deviceIndex;
        std::wcout << std::endl;

        // init endpoints
        CASM::Device endPoint = deviceManager.getDevice(deviceIndex);
        // TODO: add flag for forcing renaming
        CASM::File file("recording.wav");

        CASM::Stream streamToFile(endPoint, file, std::chrono::milliseconds(500));
        std::wcout << L"Recording ..";
        streamToFile.start();
        // request audio stream waiting for timeout before closing
        streamToFile.stop(std::chrono::seconds(6));
        // waiting for audio stream finished
        // 1st way, wait in place
        streamToFile.join();
        // 2nd way, check for state
        /*while(streamToFile.IsActive()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            std::cout << ".";
        }*/

        CASM::Stream streamFromFile(file, endPoint, std::chrono::milliseconds(500));
        streamFromFile.start();
        // we are not forcing stop, just waiting for end of file
        streamFromFile.join();
    } catch (std::exception& e) {
        std::wcout << e.what();
    }

    return 0;
}
