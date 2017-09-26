#===================================
#============= BUILD ===============
#===================================

find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen was not found on your system")
endif()

# set input and output files
set(DOXYGEN_IN Doxyfile.in)
set(DOXYGEN_OUT ${casm_doc_output_dir}/Doxyfile)

# request to configure the file
configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
message("Doxygen build started at ${casm_doc_output_dir}")

# note the option ALL which allows to build the docs together with the application
add_custom_target( casm_docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${casm_doc_output_dir}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM )
