#include <CASM/CASM.hpp>
#include <CASM/stream.hpp>
#include <CASM/codec/pcm.hpp>
#include <CASM/device_api/wasapi.hpp>
#include <iostream>

#define CASM_MANUAL 0

int
main() {
    try {
        // Choose device
        CASM::DeviceApi::Wasapi::Enumerator deviceEnumerator;
        CASM::DeviceManager deviceManager(std::move(deviceEnumerator));
        deviceManager.update();
        int deviceCount = deviceManager.getDeviceCount();
        for (unsigned int i = 0; i < deviceCount; i++) {
            std::cout << i << ": " << deviceManager.getDevice(i).getName() << std::endl;
        }

        // get device index
        unsigned int deviceIndex;
        std::cout << "Choose device index: ";
        std::cin >> deviceIndex;
        std::cout << std::endl;

        // Init endpoints
        CASM::Device endPoint = deviceManager.getDevice(deviceIndex);
        CASM::Codec::Pcm codec;
        CASM::DataStream::Fstream dataStream;
        auto filename = CASM::Util::Filesystem::generateNextNameIfExist("recording.wav");
        CASM::File file(std::move(codec), std::move(dataStream), filename);

        // Stream to file
        CASM::Stream streamToFile(endPoint, file, std::chrono::milliseconds(500));
        std::cout << "Recording ..";
        streamToFile.start();
        // Request audio stream waiting for timeout before stop recording
        streamToFile.stop(std::chrono::seconds(6));
        // Waiting for audio stream finished
#if !CASM_MANUAL
        // 1st way, wait in place
        streamToFile.join();
#else
        // 2nd way, check for state
        while(streamToFile.IsActive()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            std::cout << ".";
        }
#endif

        // Play file
        CASM::Stream streamFromFile(file, endPoint, std::chrono::milliseconds(500));
        streamFromFile.start();
        // Waiting till the end of file
        streamFromFile.join();
    } catch (std::exception& e) {
        std::cout << e.what();
    }

    return 0;
}
