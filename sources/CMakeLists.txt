#===================================
#========== LIBRARY NAME ===========
#===================================
set(CMAKE_DEBUG_POSTFIX d)

#===================================
#======== LIBRARY SOURCES ==========
#===================================
file(GLOB library_sources
    core/buffer.cpp
    device_manager/device.cpp
    core/wave_properties.cpp
    device_manager/device_manager.cpp
    device_manager/device_manager_base.cpp
    file.cpp

    device_manager/windows/device_windows_wasapi.cpp
    device_manager/windows/device_manager_windows_wasapi.cpp
    )

#===================================
#============== BUILD ==============
#===================================
if(CASM_STATIC)
    set(CASM_BUILD_FLAG STATIC)
else()
    set(CASM_BUILD_FLAG SHARED)
endif()

add_library(${CASM_LIBRARY} ${CASM_BUILD_FLAG} ${library_sources})
