#===================================
#========== LIBRARY NAME ===========
#===================================
set(CMAKE_DEBUG_POSTFIX d)

#===================================
#======== LIBRARY SOURCES ==========
#===================================
file(GLOB library_headers
    ${PROJECT_SOURCE_DIR}/include/CASM/*.hpp
    ${PROJECT_SOURCE_DIR}/include/CASM/*/*.hpp
    )

file(GLOB library_sources
    file.cpp
    core/*.cpp
    stream.cpp
    file/file_base.cpp
    file/file_wave.cpp
    device_manager/device.cpp
    device_manager/device_manager.cpp
    device_manager/device_manager_base.cpp
    device_manager/windows/windows_utilities.cpp
    device_manager/windows/device_windows_wasapi.cpp
    device_manager/windows/device_manager_windows_wasapi.cpp
    device_manager/windows/initguid.cpp
    )

#===================================
#============== BUILD ==============
#===================================
if(CASM_STATIC)
    set(CASM_BUILD_FLAG STATIC)
else()
    set(CASM_BUILD_FLAG SHARED)
endif()

add_library(${CASM_LIBRARY} ${CASM_BUILD_FLAG} ${library_sources} ${library_headers})
target_link_libraries(${CASM_LIBRARY} uuid.lib ole32.lib)

if(CASM_BUILD_ENV_MSBUILD)
    target_compile_definitions(${CASM_LIBRARY} PRIVATE CASM_BUILD_ENV_MSBUILD=1)
endif()
