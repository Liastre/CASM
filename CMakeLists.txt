cmake_minimum_required(VERSION 3.7)
project(CrossAudioStreamManager)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(record_endpoint_device
    record_endpoint_device.cpp
    sources/file.cpp
    sources/buffer.cpp
    sources/wave_properties.cpp
    sources/device_windows_wasapi.cpp
    sources/device_manager_windows_wasapi.cpp
    )

add_executable(create_wav_output
    create_wav_output.cpp
    sources/file.cpp
    sources/buffer.cpp
    sources/wave_properties.cpp
    )

add_executable(WASAPIRecord
    CmdLine.cpp
    WASAPICapture.cpp
    WASAPICaptureSharedTimerDriven.cpp
    sources/file.cpp
    sources/buffer.cpp
    sources/wave_properties.cpp
    sources/device_manager_windows_wasapi.cpp
    )
target_link_libraries(WASAPIRecord avrt)

add_executable(read_wav
    sources/wave_properties.cpp
    sources/buffer.cpp
    sources/file.cpp
    read_wav.cpp)
