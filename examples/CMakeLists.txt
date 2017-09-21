#===================================
#============= BUILD ===============
#===================================
# Windows Vista and Higher examples
if(CASM_WIN32_WASAPI)
    # record_then_play
    add_executable(record_then_play record_then_play.cpp)
    target_link_libraries(record_then_play ${CASM_LIBRARY})

    # record_endpoint_device
    add_executable(record_endpoint_device record_endpoint_device.cpp)
    target_link_libraries(record_endpoint_device ${CASM_LIBRARY})

    # play_wav_file
    add_executable(play_wav_file play_wav_file.cpp)
    target_link_libraries(play_wav_file ${CASM_LIBRARY})
endif()
