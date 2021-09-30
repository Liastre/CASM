#===================================
#============= SYSTEM ==============
#===================================
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)

#===================================
#============ COMPILER =============
#===================================

#===================================
#============= PROJECT =============
#===================================
project(CASM) # Crossplatform Audio Stream Manager
set(CASM_VERSION_MAJOR "0")
set(CASM_VERSION_MINOR "2")
set(CASM_VERSION_PATCH "0")
set(CASM_VERSION_EXTRA "")
set(CASM_LIBRARY ${PROJECT_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#===================================
#=========== CONFIGURE =============
#===================================
# options
option(CASM_SHARED "Build shared library" OFF)
option(CASM_BUILD_EXAMPLES "Build examples" ON)
option(CASM_BUILD_DOC "Build documentation" OFF)

# detect build definitions
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(CASM_BUILD_ENV_MSBUILD TRUE)
    message(STATUS "That's visual studio")
elseif(MINGW)
    set(CASM_BUILD_ENV_MINGW TRUE)
    message(STATUS "Build environment is set to MinGW")
endif()

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
set(casm_binary_output_dir         ${casm_build_output_dir}/binary)
set(casm_doc_output_dir            ${casm_build_output_dir}/docs)
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

# documentation
if(CASM_BUILD_DOC)
    add_subdirectory(docs)
endif()
