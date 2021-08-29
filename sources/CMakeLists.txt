#===================================
#========== LIBRARY NAME ===========
#===================================
set(CMAKE_DEBUG_POSTFIX d)

#===================================
#======== LIBRARY SOURCES ==========
#===================================
file(GLOB casm_headers
    ${PROJECT_SOURCE_DIR}/include/CASM/*.hpp
    ${PROJECT_SOURCE_DIR}/include/CASM/*/*.hpp
    ${PROJECT_SOURCE_DIR}/include/CASM/*/*.inl
    )

file(GLOB casm_sources
    file.cpp
    stream.cpp
    device.cpp
    device_manager.cpp
    core/*.cpp
    codec/pcm.cpp
    data_stream/fstream.cpp
    device_api/enumerator_base.cpp
    device_api/wasapi/windows_utilities.cpp
    device_api/wasapi/device.cpp
    device_api/wasapi/enumerator.cpp
    device_api/wasapi/initguid.cpp
    )

#===================================
#============== BUILD ==============
#===================================
if(CASM_SHARED)
    set(CASM_BUILD_FLAG SHARED)
else()
    set(CASM_BUILD_FLAG STATIC)
endif()

add_library(${CASM_LIBRARY} ${CASM_BUILD_FLAG} ${casm_sources} ${casm_headers})
target_link_libraries(${CASM_LIBRARY} uuid.lib ole32.lib)

if(CASM_BUILD_ENV_MSBUILD)
    target_compile_definitions(${CASM_LIBRARY} PRIVATE CASM_BUILD_ENV_MSBUILD=1)
endif()
