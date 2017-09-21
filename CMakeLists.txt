#===================================
#============= SYSTEM ==============
#===================================
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)

#===================================
#============ COMPILER =============
#===================================

#===================================
#============= PROJECT =============
#===================================
project(CASM) # Crossplatform Audio Stream Manager
set(CASM_VERSION_MAJOR "0")
set(CASM_VERSION_MINOR "1")
set(CASM_VERSION_PATCH "0")
set(CASM_VERSION_EXTRA "")
set(CASM_LIBRARY ${PROJECT_NAME})

#===================================
#=========== CONFIGURE =============
#===================================
# options
option(CASM_STATIC "Build shared library" OFF)
option(CASM_BUILD_EXAMPLES "Build examples" ON)

# detect system definitions
if(WIN32) # windows (32 and 64 bit)
    if(${CMAKE_SYSTEM_VERSION} LESS 5.0)
        set(CASM_WIN32_UNKNOWN TRUE)
        message(FATAL_ERROR "Unsupported version of Windows")
    endif()
    # windows XP or greater
    if(${CMAKE_SYSTEM_VERSION} GREATER_EQUAL 5.0)
        set(CASM_WIN32_KERNELSTREAMING TRUE)
    endif()
    # windows Vista or greater
    if(${CMAKE_SYSTEM_VERSION} GREATER_EQUAL 6.0)
        set(CASM_WIN32_WASAPI TRUE)
    endif()
elseif()
    message(FATAL_ERROR "Unsupported operating system")
endif()

#===================================
#============= OUTPUT ==============
#===================================
set(casm_build_output_dir          ${CMAKE_BINARY_DIR})
set(casm_binary_output_dir         ${CMAKE_BINARY_DIR}/binary)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${casm_binary_output_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${casm_binary_output_dir})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${casm_binary_output_dir})

#===================================
#============= BUILD ===============
#===================================
# library
include_directories(include)
add_subdirectory(sources)

# examples
if(CASM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
