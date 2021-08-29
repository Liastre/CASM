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
        std::size_t deviceCount = deviceManager.getDeviceCount();
        for (std::size_t i = 0; i < deviceCount; i++) {
            std::cout << i << ") " << deviceManager.getDevice(i).getDescription() << std::endl;
        }
        std::size_t deviceIndex(0);
        std::cout << "Choose device index: ";
        std::cin >> deviceIndex;
        std::cout << std::endl;

        // Init endpoints
        CASM::Device endPoint = deviceManager.getDevice(deviceIndex);
        auto endPointProperties = endPoint.getDeviceWaveProperties();
        std::stringstream ss;
        ss << endPoint.getDescription()
           << '_' << endPointProperties.getSamplesPerSecond() << "Hz"
           << '_' << endPointProperties.getBitsPerSample() << "bit"
           << '_' << endPointProperties.getChannelsCount() << "ch"
           << ".wav";
        auto str = ss.str();
        CASM::Codec::Pcm codec;
        CASM::DataStream::Fstream dataStream;
        CASM::File outputFile(std::move(codec), std::move(dataStream), ss.str());
        if (!outputFile) {
            std::cout << "Unable to create file" << std::endl;
            return 1;
        }

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
        std::cout << e.what();
    }

    return 0;
}
