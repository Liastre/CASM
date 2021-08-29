#include <CASM/CASM.hpp>
#include <CASM/device_manager.hpp>
#include <CASM/stream.hpp>
#include <CASM/codec/pcm.hpp>
#include <CASM/device_api/wasapi.hpp>
#include <iostream>
#include <Windows.h>

#define CASM_MANUAL 0

int
main(int argc, char** argv) {
    SetConsoleOutputCP(65001);
    try {
        // Choose device
        CASM::DeviceApi::Wasapi::Enumerator deviceEnumerator;
        CASM::DeviceManager deviceManager(std::move(deviceEnumerator));
        deviceManager.update();
        std::size_t deviceCount = deviceManager.getDeviceCount();
        for (std::size_t i = 0; i < deviceCount; i++) {
            auto deviceName = deviceManager.getDevice(i).getName();
            std::cout << i << ") " << deviceName << std::endl;
        }

        // get device index
        std::size_t deviceIndex(0);
        std::cout << "Choose device index: ";
        std::cin >> deviceIndex;
        std::cout << std::endl;

        // init endpoints
        CASM::Device outputDevice = deviceManager.getDevice(deviceIndex);
        if (!outputDevice) {
            std::cout << "Output device is not valid!" << std::endl;
            return 0;
        } else {
            auto deviceWaveProp = outputDevice.getStreamWaveProperties();
            std::cout << "Device properties of " << outputDevice.getDescription()
                       << "\nChannels count: " << deviceWaveProp.getChannelsCount()
                       << "\nSamples per second: " << deviceWaveProp.getSamplesPerSecond()
                       << "\nBits per sample: " << deviceWaveProp.getBitsPerSample()
                       << std::endl;
        }
        CASM::Codec::Pcm codec;
        CASM::DataStream::Fstream dataStream;
        CASM::File inputFile(std::move(codec), std::move(dataStream), "D:\\Development\\projects\\Application_CrossplatformAudioStreamManager\\data\\250Hz_48000Hz_32bit_2ch_30sec.wav");
        if (!inputFile) {
            std::cout << "Input file is not valid!" << std::endl;
            return 0;
        }

#if !CASM_MANUAL
        // Way 1: using audio Stream
        CASM::Stream streamFromFile(inputFile, outputDevice, std::chrono::milliseconds{ 500 });
        streamFromFile.start();
        // We are not forcing stop, just waiting for end of input endpoint
        // in our case it's the same with file end
        streamFromFile.join();
        std::cout << "Stream uptime is: " << std::chrono::duration_cast<std::chrono::seconds>(streamFromFile.getUptime()).count();
#else
        // Way 2: manual streaming
        // Open endpoints
        CASM::Buffer fileBuffer;
        inputFile.openCaptureStream(std::chrono::milliseconds{ 500 }, fileBuffer);
        outputDevice.openRenderStream(fileBuffer);
        CASM::BufferStatus status(CASM::BufferStatus::BufferFilled);
        int counter(0);
        // Manual read-write
        while (status != CASM::BufferStatus::DataEmpty) {
            status = inputFile.read(fileBuffer);
            counter += fileBuffer.getSize();
            outputDevice.write(fileBuffer);
        }
        std::wcout << L"Counter: " << counter;
        // Close devices
        inputFile.closeRenderStream();
        outputDevice.closeCaptureStream();
#endif
    } catch (std::exception& e) {
        std::cout << e.what();
    } catch (...) {
        // unexpected errors
        std::cout << "Unexpected error has occurred. Program forced to quit." << std::endl;
    }

    return 0;
}
