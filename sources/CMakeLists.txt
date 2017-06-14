#===================================
#========== LIBRARY NAME ===========
#===================================
set(CMAKE_DEBUG_POSTFIX d)

#===================================
#======== LIBRARY SOURCES ==========
#===================================
file(GLOB library_sources
    wave_properties.cpp
    device_manager_windows_wasapi.cpp
    )

#===================================
#============== BUILD ==============
#===================================
if(ANTTWEAKBAR_SHARED)
    set(ANTTWEAKBAR_BUILD_FLAG SHARED)
else()
    set(ANTTWEAKBAR_BUILD_FLAG STATIC)
endif()

add_library(${ANTTWEAKBAR_LIBRARY} ${ANTTWEAKBAR_BUILD_FLAG} ${library_sources})
