#pragma once

/**
 * @brief OS configuration header
 * @author Liastre
 * @copyright MIT
 *
 * Available user defines description:
 * CASM_DEBUG
 * description:
 *      debug define with 5 levels of debug messages
 * value:
 *      integer value from 0 to 5
 *
 *
 * Config produced defines:
 * PLATFORM
 * description:
 *      defines platform we building code on
 * value:
 *      CASM_PLATFORM_APPLE
 *      CASM_PLATFORM_GOOGLE
 *      CASM_PLATFORM_MICROSOFT
 *      CASM_PLATFORM_EMSCRIPTEN
 *
 * OPERATING SYSTEM
 * description:
 *      defines operating system we building code on
 * value:
 *      CASM_OS_IOS
 *      CASM_OS_IOS_SIMULATOR
 *      CASM_OS_MACOS
 *      CASM_OS_ANDROID
 *      CASM_OS_WINDOWS
 *
 * FEATURES
 * description:
 *     features available on platform
 * values:
 *      CASM_FEATURE_CPP_VERSION - C++ standard version
 *      CASM_FEATURE_CPP_17
 *      CASM_FEATURE_CPP_17_EX - experimental
 *
 * BUILD
 * description:
 *      specific build type defines
 * values:
 *      CASM_BUILD_X86
 *      CASM_BUILD_X86_64
 *
 * APPTYPE
 * description:
 *      specific application type defines, depends on platform
 * values:
 *      CASM_APPTYPE_UWP - Universal Windows Platform (c++/winrt)
 *      CASM_APPTYPE_DESKTOP
 */

// common macroses
#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)
#define WARNING(desc) message(__FILE__ "(" STRINGIZE(__LINE__) "): warning custom: " desc)

// os defines
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
#    define CASM_PLATFORM_MICROSOFT 1
#    define CASM_OS_WINDOWS 1
#    define CASM_BUILD_X86_64 1
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
#    define CASM_PLATFORM_MICROSOFT 1
#    define CASM_OS_WINDOWS 1
#    define CASM_BUILD_X86 1
#endif

#if defined(CASM_OS_WINDOWS)
#    define NOMINMAX
#    include <Windows.h>
#    if defined(WINAPI_FAMILY_PC_APP)
#        define CASM_APPTYPE_UWP 1
#    endif
#    undef ERROR
#endif

#if defined(__APPLE__)
#    include <TargetConditionals.h>
#    define CASM_PLATFORM_APPLE 1
#    if TARGET_OS_SIMULATOR > 0
#        define CASM_OS_IOS 1
#        define CASM_OS_IOS_SIMULATOR 1
#    elif TARGET_OS_IPHONE > 0
#        define CASM_OS_IOS 1
#    elif TARGET_OS_MAC > 0
#        define CASM_OS_MACOS 1
#    endif
#endif

#if defined(__ANDROID__)
#    define CASM_PLATFORM_GOOGLE 1
#    define CASM_OS_ANDROID 1
#endif

#if defined(__EMSCRIPTEN__)
#    define CASM_PLATFORM_EMSCRIPTEN 1
#endif

// feature defines
#if CASM_PLATFORM_MICROSOFT
#    ifndef _MSVC_LANG
#        define _MSVC_LANG 199711L
#    endif
#    define CASM_FEATURE_CPP_VERSION _MSVC_LANG
#    if _MSC_VER >= 1800 /*VS2013*/ && _MSC_VER < 1900 /*VS2015*/
#        define PRIVATE_CASM_FEATURE_CPP_11_EX 1
#    endif
#    if _MSC_VER > 1910 && _MSC_VER <= 1924
#        define PRIVATE_CASM_FEATURE_CPP_17_EX 1
#    endif
#    if defined __cplusplus_winrt
#        define PRIVATE_CASM_FEATURE_WINDOWS_ZW 1
#    endif
#else
#    define CASM_FEATURE_CPP_VERSION __cplusplus
#endif

#if CASM_FEATURE_CPP_VERSION >= 201103L
#    define CASM_FEATURE_CPP_11 1
#elif PRIVATE_CASM_FEATURE_CPP_11_EX
#    define CASM_FEATURE_CPP_11 1
#    define CASM_FEATURE_CPP_11_EX 1
#endif

#if CASM_FEATURE_CPP_VERSION >= 201703L
#    define CASM_FEATURE_CPP_17 1
#elif PRIVATE_CASM_FEATURE_CPP_17_EX
#    define CASM_FEATURE_CPP_17 1
#    define CASM_FEATURE_CPP_17_EX 1
#endif

#if CASM_PLATFORM_EMSCRIPTEN && !__EMSCRIPTEN_PTHREADS__
#    define CASM_FEATURE_THREADS 0
#else
#    define CASM_FEATURE_THREADS 1
#endif

// build defines
#if defined(DEBUG) || defined(_DEBUG) && !defined(CASM_DEBUG)
#    define CASM_DEBUG 1
#endif

#if !CASM_DEBUG
#    ifndef NDEBUG
#        define NDEBUG 1
#    endif
#endif

// system defines and includes
#if defined(_CPPRTTI)
#    pragma WARNING("RTTI is enabled by default, make sure you disabled it")
#endif

#if CASM_PLATFORM_MICROSOFT && CASM_FEATURE_CPP_11_EX
#    ifndef noexcept
#        define noexcept throw()
#    endif
#endif

#if CASM_OS_WINDOWS
#    define stat _stat
#    undef min
#    undef max
#else
#    include <unistd.h>
#    include <sys/stat.h>
#endif
