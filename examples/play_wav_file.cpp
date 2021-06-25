#include <CASM/CASM.hpp>
#include <CASM/device_manager.hpp>
#include <CASM/stream.hpp>
#include <CASM/file.hpp>
#include <iostream>
#include <thread>
#include <string>
#include <io.h>
#include <fcntl.h>

int
main(int argc, char** argv) {
    try {
        // TODO temp solution for cmd
        _setmode(_fileno(stdout), _O_U16TEXT);

        // choose device
        CASM::DeviceManager deviceManager;
        deviceManager.update();
        std::size_t deviceCount = deviceManager.getDeviceCount();
        for (std::size_t i = 0; i < deviceCount; i++) {
            auto deviceName = deviceManager.getDevice(i).getDescription();
            std::wcout << i << ": " << deviceName << std::endl;
        }

        // get device index
        std::size_t deviceIndex;
        std::wcout << L"Choose device index ...";
        std::wcin >> deviceIndex;
        std::wcout << std::endl;

        // init endpoints
        CASM::Device outputDevice = deviceManager.getDevice(deviceIndex);
        if (!outputDevice) {
            std::wcout << L"Output device is not valid!" << std::endl;
            return 0;
        } else {
            auto deviceWaveProp = outputDevice.getStreamWaveProperties();
            std::wcout << L"Device properties of " << outputDevice.getDescription()
                       << L"\nChannels count: " << deviceWaveProp.getChannelsCount()
                       << L"\nSamples per second: " << deviceWaveProp.getSamplesPerSecond()
                       << L"\nBits per sample: " << deviceWaveProp.getBitsPerSample()
                       << std::endl;
        }

        CASM::File inputFile("D:\\Development\\projects\\Application_CrossplatformAudioStreamManager\\data\\250Hz_48000Hz_32bit_2ch_30sec.wav");
        if (!inputFile) {
            std::wcout << L"Input file is not valid!" << std::endl;
            return 0;
        }

        // Way 1: using audio Stream
        CASM::Stream streamFromFile(inputFile, outputDevice, std::chrono::milliseconds{ 500 });
        streamFromFile.start();
        // We are not forcing stop, just waiting for end of file
        streamFromFile.join();
        std::wcout << L"Stream uptime is: " << std::chrono::duration_cast<std::chrono::seconds>(streamFromFile.getUptime()).count();

        // Way 2: manual streaming
        // Open endpoints
        /*
        CASM::Buffer fileBuffer;
        inputFile.openCaptureStream(std::chrono::milliseconds{ 500 }, fileBuffer);
        outputDevice.openRenderStream(fileBuffer);
        CASM::BufferStatus status(CASM::BufferStatus::BufferFilled);
        int counter(0);
        while (status != CASM::BufferStatus::DataEmpty) {
            status = inputFile.read(fileBuffer);
            counter += fileBuffer.getSize();
            outputDevice.write(fileBuffer);
        }
        std::wcout << L"Counter: " << counter;
        inputFile.closeRenderStream();
        outputDevice.closeCaptureStream();
        */
    } catch (std::exception& e) {
        std::wcout << e.what();
    } catch (...) {
        // unexpected errors
        std::wcout << L"Unexpected error has occurred. Program forced to quit." << std::endl;
    }

    return 0;
}
