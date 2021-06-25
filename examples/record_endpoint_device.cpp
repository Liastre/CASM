#include <CASM/CASM.hpp>
#include <CASM/device_manager.hpp>
#include <CASM/file.hpp>
#include <CASM/stream.hpp>
#include <iostream>
#include <thread>
#include <io.h>
#include <fcntl.h>

#define CASM_MANUAL 0

int
main() {
    try {
        // TODO temp solution for cmd
        _setmode(_fileno(stdout), _O_U16TEXT);

        // Choose device
        CASM::DeviceManager deviceManager;
        deviceManager.update();
        std::size_t deviceCount = deviceManager.getDeviceCount();
        for (std::size_t i = 0; i < deviceCount; i++) {
            std::wcout << i << ": " << deviceManager.getDevice(i).getDescription() << std::endl;
        }
        unsigned int deviceIndex;
        std::wcout << L"Choose device index ...";
        std::cin >> deviceIndex;

        // init endpoints
        CASM::Device endPoint = deviceManager.getDevice(deviceIndex);
        auto endPointProperties = endPoint.getDeviceWaveProperties();
        std::wstringstream ss;
        ss << endPoint.getDescription()
           << '_' << endPointProperties.getSamplesPerSecond() << L"Hz"
           << '_' << endPointProperties.getBitsPerSample() << L"bit"
           << '_' << endPointProperties.getChannelsCount() << L"ch";
        auto filename = ss.str();
        //250Hz_48000Hz_32bit_2ch_30sec.wav
        CASM::File outputFile("endPointDevice.wav");
        //if (!file) {
        //    std::wcout << L"Unable to create file" << std::endl;
        //    return 1;
        //}


#if !CASM_MANUAL
        CASM::Stream streamToFile(endPoint, outputFile, std::chrono::milliseconds{ 500 });
        streamToFile.start();
        streamToFile.stop(std::chrono::seconds(10));
#else
        // Open endpoints
        CASM::Buffer buffer;
        endPoint.openCaptureStream(std::chrono::seconds(1), buffer);
        outputFile.openRenderStream(buffer);
        // Write data
        // Write 10 times by half of the buffer, with the duration
        // of buffer 1s length of the record will be 0.5*10=5 seconds
        for (int i = 0; i < 10; i++) {
            std::this_thread::sleep_for(buffer.getDuration() / 2);
            endPoint.read(buffer);
            outputFile.write(buffer);
            buffer.clear();
        }
        // Close endpoints
        outputFile.closeRenderStream();
        endPoint.closeCaptureStream();
#endif
    } catch (std::exception& e) {
        std::wcout << e.what();
    }

    return 0;
}
