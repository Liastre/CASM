// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//

//
// WASAPICaptureSharedTimerDriven.cpp : Scaffolding associated with the WASAPI Capture Shared Timer Driven sample application.
//
//  This application captures data from the specified input device and writes it to a uniquely named .WAV file in the current directory.
//

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <iostream>

#include "targetver.h"
#include <windows.h>
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#include <CASM/CASM.hpp>
#include <cassert>
#include "sources/device_manager_windows_wasapi.hpp"
#include "functiondiscoverykeys_devpkey.h"
#include "WASAPICapture.h"
#include "CmdLine.h"
#include "sources/file.hpp"

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

int TargetLatency = 20;
int TargetDurationInSec = 10;
bool ShowHelp;
bool UseConsoleDevice;
bool UseCommunicationsDevice;
bool UseMultimediaDevice;
bool DisableMMCSS;

wchar_t *OutputEndpoint;

CommandLineSwitch CmdLineArgs[] = 
{
    { L"?", L"Print this help", CommandLineSwitch::SwitchTypeNone, reinterpret_cast<void **>(&ShowHelp)},
    { L"h", L"Print this help", CommandLineSwitch::SwitchTypeNone, reinterpret_cast<void **>(&ShowHelp)},
    { L"l", L"Audio Capture Latency (ms)", CommandLineSwitch::SwitchTypeInteger, reinterpret_cast<void **>(&TargetLatency), false},
    { L"d", L"Audio Capture Duration (s)", CommandLineSwitch::SwitchTypeInteger, reinterpret_cast<void **>(&TargetDurationInSec), false},
    { L"m", L"Disable the use of MMCSS", CommandLineSwitch::SwitchTypeNone, reinterpret_cast<void **>(&DisableMMCSS)},
    { L"console", L"Use the default console device", CommandLineSwitch::SwitchTypeNone, reinterpret_cast<void **>(&UseConsoleDevice)},
    { L"communications", L"Use the default communications device", CommandLineSwitch::SwitchTypeNone, reinterpret_cast<void **>(&UseCommunicationsDevice)},
    { L"multimedia", L"Use the default multimedia device", CommandLineSwitch::SwitchTypeNone, reinterpret_cast<void **>(&UseMultimediaDevice)},
    { L"endpoint", L"Use the specified endpoint ID", CommandLineSwitch::SwitchTypeString, reinterpret_cast<void **>(&OutputEndpoint), true},
};

size_t CmdLineArgLength = ARRAYSIZE(CmdLineArgs);

//  Print help for the sample
void Help(LPCWSTR ProgramName)
{
    std::printf("Usage: %ls [-/][Switch][:][Value]\n\n", ProgramName);
    std::printf("Where Switch is one of the following: \n");
    for (size_t i = 0 ; i < CmdLineArgLength ; i += 1)
    {
        std::printf("    -%ls: %ls\n", CmdLineArgs[i].SwitchName, CmdLineArgs[i].SwitchHelp);
    }
}

//  Retrieves the device friendly name for a particular device in a device collection.
//
//  The returned string was allocated using malloc() so it should be freed using free();
//
wchar_t* GetDeviceName(IMMDeviceCollection *DeviceCollection, UINT DeviceIndex)
{
    IMMDevice *device;
    LPWSTR deviceId;
    HRESULT hr;

    hr = DeviceCollection->Item(DeviceIndex, &device);
    assert(hr==S_OK);

    hr = device->GetId(&deviceId);
    assert(hr==S_OK);

    IPropertyStore *propertyStore;
    hr = device->OpenPropertyStore(STGM_READ, &propertyStore);
    SafeRelease(&device);
    assert(hr==S_OK);

    PROPVARIANT friendlyName;
    PropVariantInit(&friendlyName);
    hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
    SafeRelease(&propertyStore);
    assert(hr==S_OK);

    wchar_t deviceName[128];
    std::swprintf(deviceName, sizeof(deviceName), L"%ls (%ls)", friendlyName.pwszVal, deviceId);

    PropVariantClear(&friendlyName);
    CoTaskMemFree(deviceId);

    wchar_t *returnValue = wcsdup(deviceName);
    //returnValue = new wchar_t(std::wcslen(deviceName)+1);
    //std::wcscpy(returnValue,deviceName);

    if (returnValue == NULL)
    {
        std::printf("Unable to allocate buffer for return\n");
        return NULL;
    }
    return returnValue;
}
//
//  Based on the input switches, pick the specified device to use.
//
bool PickDevice(IMMDevice **DeviceToUse, bool *IsDefaultDevice, ERole *DefaultDeviceRole)
{
    HRESULT hr;
    bool retValue = true;
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    IMMDeviceCollection *deviceCollection = NULL;

    *IsDefaultDevice = false;   // Assume we're not using the default device.

    hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
    if (FAILED(hr))
    {
        std::printf("Unable to instantiate device enumerator: %x\n", hr);
        retValue = false;
        SafeRelease(&deviceCollection);
        SafeRelease(&deviceEnumerator);

        return retValue;
    }

    IMMDevice *device = NULL;

    //  First off, if none of the console switches was specified, use the console device.
    if (!UseConsoleDevice && !UseCommunicationsDevice && !UseMultimediaDevice && OutputEndpoint == NULL)
    {
        //  The user didn't specify an output device, prompt the user for a device and use that.
        hr = deviceEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &deviceCollection);
        if (FAILED(hr))
        {
            std::printf("Unable to retrieve device collection: %x\n", hr);
            retValue = false;
            SafeRelease(&deviceCollection);
            SafeRelease(&deviceEnumerator);

            return retValue;
        }

        std::printf("Select an output device:\n");
        std::printf("    0:  Default Console Device\n");
        std::printf("    1:  Default Communications Device\n");
        std::printf("    2:  Default Multimedia Device\n");
        UINT deviceCount;
        hr = deviceCollection->GetCount(&deviceCount);
        assert(hr==S_OK);
        for (UINT i = 0 ; i < deviceCount ; i += 1)
        {
            wchar_t* deviceName;

            deviceName = GetDeviceName(deviceCollection, i);
            if (deviceName == NULL)
            {
                retValue = false;
                SafeRelease(&deviceCollection);
                SafeRelease(&deviceEnumerator);

                return retValue;
            }
            std::wprintf(L"    %d:  %ls\n", i + 3, deviceName);
            free(deviceName);
        }
        wchar_t choice[10];
        std::wcin >> choice;
        //_getws_s(choice);   // Note: Using the safe CRT version of _getws.

        long deviceIndex;
        wchar_t *endPointer;

        deviceIndex = wcstoul(choice, &endPointer, 0);
        if (deviceIndex == 0 && endPointer == choice)
        {
            std::printf("unrecognized device index: %ls\n", choice);
            retValue = false;
            SafeRelease(&deviceCollection);
            SafeRelease(&deviceEnumerator);

            return retValue;
        }
        switch (deviceIndex)
        {
        case 0:
            UseConsoleDevice = 1;
            break;
        case 1:
            UseCommunicationsDevice = 1;
            break;
        case 2:
            UseMultimediaDevice = 1;
            break;
        default:
            hr = deviceCollection->Item(deviceIndex - 3, &device);
            if (FAILED(hr))
            {
                std::printf("Unable to retrieve device %d: %x\n", deviceIndex - 3, hr);
                retValue = false;
                SafeRelease(&deviceCollection);
                SafeRelease(&deviceEnumerator);

                return retValue;
            }
            break;
        }
    } 
    else if (OutputEndpoint != NULL)
    {
        hr = deviceEnumerator->GetDevice(OutputEndpoint, &device);
        if (FAILED(hr))
        {
            std::printf("Unable to get endpoint for endpoint %ls: %x\n", OutputEndpoint, hr);
            retValue = false;
            SafeRelease(&deviceCollection);
            SafeRelease(&deviceEnumerator);

            return retValue;
        }
    }

    if (device == NULL)
    {
        ERole deviceRole = eConsole;    // Assume we're using the console role.
        if (UseConsoleDevice)
        {
            deviceRole = eConsole;
        }
        else if (UseCommunicationsDevice)
        {
            deviceRole = eCommunications;
        }
        else if (UseMultimediaDevice)
        {
            deviceRole = eMultimedia;
        }
        hr = deviceEnumerator->GetDefaultAudioEndpoint(eCapture, deviceRole, &device);
        if (FAILED(hr))
        {
            std::printf("Unable to get default device for role %d: %x\n", deviceRole, hr);
            retValue = false;
            SafeRelease(&deviceCollection);
            SafeRelease(&deviceEnumerator);

            return retValue;
        }
        *IsDefaultDevice = true;
        *DefaultDeviceRole = deviceRole;
    }

    *DeviceToUse = device;
    retValue = true;

    SafeRelease(&deviceCollection);
    SafeRelease(&deviceEnumerator);

    return retValue;
}

//  WAV file writer.
//
//  This is a VERY simple .WAV file writer.
//

//
//  A wave file consists of:
//
//  RIFF header:    8 bytes consisting of the signature "RIFF" followed by a 4 byte file length.
//  WAVE header:    4 bytes consisting of the signature "WAVE".
//  fmt header:     4 bytes consisting of the signature "fmt " followed by a WAVEFORMATEX 
//  WAVEFORMAT:     <n> bytes containing a waveformat structure.
//  DATA header:    8 bytes consisting of the signature "data" followed by a 4 byte file length.
//  wave data:      <m> bytes containing wave data.
//
//
//  Header for a WAV file - we define a structure describing the first few fields in the header for convenience.
//
struct WAVEHEADER
{
    DWORD   dwRiff;                     // "RIFF"
    DWORD   dwSize;                     // Size
    DWORD   dwWave;                     // "WAVE"
    DWORD   dwFmt;                      // "fmt "
    DWORD   dwFmtSize;                  // Wave Format Size
};

//  Static RIFF header, we'll append the format to it.
const BYTE WaveHeader[] = 
{
    'R',   'I',   'F',   'F',  0x00,  0x00,  0x00,  0x00, 'W',   'A',   'V',   'E',   'f',   'm',   't',   ' ', 0x00, 0x00, 0x00, 0x00
};

//  Static wave DATA tag.
const BYTE WaveData[] = { 'd', 'a', 't', 'a'};

//
//  Write the contents of a WAV file.  We take as input the data to write and the format of that data.
//
bool WriteWaveFile(HANDLE FileHandle, const BYTE *Buffer, const size_t BufferSize, const WAVEFORMATEX *WaveFormat)
{
    DWORD waveFileSize = sizeof(WAVEHEADER) + sizeof(WAVEFORMATEX) + WaveFormat->cbSize + sizeof(WaveData) + sizeof(DWORD) + static_cast<DWORD>(BufferSize);
    BYTE *waveFileData = new (std::nothrow) BYTE[waveFileSize];
    BYTE *waveFilePointer = waveFileData;
    WAVEHEADER *waveHeader = reinterpret_cast<WAVEHEADER *>(waveFileData);

    //  Copy in the wave header - we'll fix up the lengths later
    CopyMemory(waveFilePointer, WaveHeader, sizeof(WaveHeader));
    waveFilePointer += sizeof(WaveHeader);

    //  Update the sizes in the header
    waveHeader->dwSize = waveFileSize - (2 * sizeof(DWORD));
    waveHeader->dwFmtSize = sizeof(WAVEFORMATEX) + WaveFormat->cbSize;

    //  Next copy in the WaveFormatex structure
    CopyMemory(waveFilePointer, WaveFormat, sizeof(WAVEFORMATEX) + WaveFormat->cbSize);
    waveFilePointer += sizeof(WAVEFORMATEX) + WaveFormat->cbSize;


    //  Then the data header
    CopyMemory(waveFilePointer, WaveData, sizeof(WaveData));
    waveFilePointer += sizeof(WaveData);
    *(reinterpret_cast<DWORD *>(waveFilePointer)) = static_cast<DWORD>(BufferSize);
    waveFilePointer += sizeof(DWORD);

    //  And finally copy in the audio data
    CopyMemory(waveFilePointer, Buffer, BufferSize);

    //  Last but not least, write the data to the file
    DWORD bytesWritten;
    if (!WriteFile(FileHandle, waveFileData, waveFileSize, &bytesWritten, NULL))
    {
        std::printf("Unable to write wave file: %d\n", GetLastError());
        delete []waveFileData;
        return false;
    }

    if (bytesWritten != waveFileSize)
    {
        std::printf("Failed to write entire wave file\n");
        delete []waveFileData;
        return false;
    }
    delete []waveFileData;
    return true;
}

//  Write the captured wave data to an output file so that it can be examined later
void SaveWaveData(BYTE *CaptureBuffer, size_t BufferSize, const WAVEFORMATEX *WaveFormat)
{
    HRESULT hr;
    wchar_t waveFileName[MAX_PATH];

    GUID testGuid;
    hr = CoCreateGuid(&testGuid);
    assert(hr==S_OK);
    wchar_t* guidString;
    hr = StringFromCLSID(testGuid, &guidString);
    assert(hr==S_OK);
    std::swprintf(waveFileName, MAX_PATH, L"%ls%ls.%ls", L"WASAPICaptureTimerDriven-", guidString, L"wav");

    HANDLE waveHandle = CreateFileW(waveFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (waveHandle != INVALID_HANDLE_VALUE)
    {
        if (WriteWaveFile(waveHandle, CaptureBuffer, BufferSize, WaveFormat))
        {
            std::printf("Successfully wrote WAVE data to %ls\n", waveFileName);
        } else {
            std::printf("Unable to write wave file\n");
        }
        CloseHandle(waveHandle);
    } else {
        std::printf("Unable to open output WAV file %ls: %d\n", waveFileName, GetLastError());
    }
    CoTaskMemFree(guidString);
}

//
//  The core of the sample.
//
//  Parse the command line, interpret the input parameters, pick an audio device then capture data from that device.
//  When done, write the data to a file.
//
int main(int argc, char* argv[])
{
    //
    //DeviceManagerWindowsWASAPI deviceManager;
    WaveProperties waveProperties(2, 44100, 32);
    File file("WASAPICaptureMy.wav", CASM::WRITE, waveProperties);
    //

    int result = 0;
    IMMDevice *device = NULL;
    bool isDefaultDevice;
    ERole role;

    std::printf("WASAPI Capture Shared Timer Driven Sample\n");
    std::printf("Copyright (c) Microsoft.  All Rights Reserved\n");
    std::printf("\n");

    std::vector<wchar_t*> arr(argc);
    for(int i=0; i<argc; i++) {
        wchar_t arg[512];
        mbstowcs(arg, argv[i], 512);
        arr[i] = arg;
    }

    if (!ParseCommandLine(argc, &arr[0], CmdLineArgs, CmdLineArgLength))
    {
        result = -1;
        SafeRelease(&device);
        CoUninitialize();
        return 0;
    }

    //  Now that we've parsed our command line, do some semantic checks.
    //  First off, show the help for the app if the user asked for it.
    if (ShowHelp)
    {
        Help(arr[0]);
        SafeRelease(&device);
        CoUninitialize();
        return 0;
    }

    //  The user can only specify one of -console, -communications or -multimedia or a specific endpoint.
    if (((UseConsoleDevice != 0) + (UseCommunicationsDevice != 0) + (UseMultimediaDevice != 0) + (OutputEndpoint != NULL)) > 1)
    {
        std::printf("Can only specify one of -Console, -Communications or -Multimedia\n");
        result = -1;
        SafeRelease(&device);
        CoUninitialize();
        return 0;
    }

    //  A GUI application should use COINIT_APARTMENTTHREADED instead of COINIT_MULTITHREADED.
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        std::printf("Unable to initialize COM: %x\n", hr);
        result = hr;
        SafeRelease(&device);
        CoUninitialize();
        return 0;
    }

    //  Now that we've parsed our command line, pick the device to capture.
    if (!PickDevice(&device, &isDefaultDevice, &role))
    {
        result = -1;
        SafeRelease(&device);
        CoUninitialize();
        return 0;
    }

    std::printf("Capture audio data for %d seconds\n", TargetDurationInSec);

    //  Instantiate a capturer and capture sounds for TargetDuration seconds
    //
    //  Configure the capturer to enable stream switching on the specified role if the user specified one of the default devices.
    {
        CWASAPICapture *capturer = new (std::nothrow) CWASAPICapture(device, isDefaultDevice, role);
        if (capturer == NULL)
        {
            std::printf("Unable to allocate capturer\n");
            return -1;
        }

        if (capturer->Initialize(TargetLatency))
        {
            //  We've initialized the capturer.  Once we've done that, we know some information about the
            //  mix format and we can allocate the buffer that we're going to capture.
            //
            //  The buffer is going to contain "TargetDuration" seconds worth of PCM data.  That means 
            //  we're going to have TargetDuration*samples/second frames multiplied by the frame size.
            size_t captureBufferSize = capturer->SamplesPerSecond() * TargetDurationInSec * capturer->FrameSize();
            BYTE *captureBuffer = new (std::nothrow) BYTE[captureBufferSize];

            if (captureBuffer == NULL)
            {
                std::printf("Unable to allocate capture buffer\n");
                return -1;
            }

            if (capturer->Start(captureBuffer, captureBufferSize))
            {
                do
                {
                    std::printf(".");
                    Sleep(1000);
                } while (--TargetDurationInSec);
                std::printf("\n");

                capturer->Stop();

                //  We've now captured our wave data.  Now write it out in a wave file.
                SaveWaveData(captureBuffer, capturer->BytesCaptured(), capturer->MixFormat());
                file.write<BYTE>(captureBuffer, capturer->BytesCaptured());
                file.finalize();

                //  Now shut down the capturer and release it we're done.
                capturer->Shutdown();
                SafeRelease(&capturer);
            }

            delete []captureBuffer;
        }
    }

    SafeRelease(&device);
    CoUninitialize();
    return 0;
}
