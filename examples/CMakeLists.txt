#===================================
#============= BUILD ===============
#===================================
set(CASM_EXAMPLES_FOLDER "Examples")

# Windows Vista and Higher examples
if(CASM_WIN32_WASAPI)
    # record_then_play
    add_executable(record_then_play record_then_play.cpp)
    target_link_libraries(record_then_play ${CASM_LIBRARY})
    set_target_properties(record_then_play PROPERTIES FOLDER ${CASM_EXAMPLES_FOLDER})

    # record_endpoint_device
    add_executable(record_endpoint_device record_endpoint_device.cpp)
    target_link_libraries(record_endpoint_device ${CASM_LIBRARY})
    set_target_properties(record_endpoint_device PROPERTIES FOLDER ${CASM_EXAMPLES_FOLDER})

    # play_wav_file
    add_executable(play_wav_file play_wav_file.cpp)
    target_link_libraries(play_wav_file ${CASM_LIBRARY})
    set_target_properties(play_wav_file PROPERTIES FOLDER ${CASM_EXAMPLES_FOLDER})
endif()
