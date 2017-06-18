#===================================
#============= BUILD ===============
#===================================
# Windows Vista and Higher examples
if(CASM_WIN32_WASAPI)
    # record_endpoint_device
    add_executable(record_endpoint_device record_endpoint_device.cpp)
    target_link_libraries(record_endpoint_device ${CASM_LIBRARY})
endif()
