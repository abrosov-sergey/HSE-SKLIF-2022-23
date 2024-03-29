cmake_minimum_required(VERSION 2.8)

project( SklyphVALT )

# Poject version
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 0)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL})

add_definitions(-DVERSION="${${PROJECT_NAME}_VERSION}")

# Path to additional cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

set(QDCM_BUILD_TESTS ON CACHE BOOL "Build unit tests")
set(QDCM_TESTS_COVERAGE OFF CACHE BOOL "Enable tests coverage via gcov")

set( CMAKE_CXX_STANDARD 11)
set( CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_CXX_EXTENSIONS OFF)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11" )

set( PROJECT_ROOT ${PROJECT_SOURCE_DIR} )
set( DESTDIR_DEBUG ${PROJECT_ROOT}/deploy/debug )
set( DESTDIR_RELEASE ${PROJECT_ROOT}/deploy/release )
set( DESTDIR ${PROJECT_ROOT}/deploy )

if ( CMAKE_BUILD_TYPE MATCHES Debug )
    set( DESTDIR ${DESTDIR_DEBUG} )
elseif( CMAKE_BUILD_TYPE MATCHES Release )
    set( DESTDIR ${DESTDIR_RELEASE} )
else()
    message( FATAL_ERROR "Unknown build configuration!" )
endif()

set(INSTALL_BIN "bin")
set(INSTALL_LIB "lib")
set(INSTALL_INCLUDE "include")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DESTDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DESTDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/target/lib)

# Assign default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release")
endif(NOT CMAKE_BUILD_TYPE)

# Building for x86_64 required -fPIC flag
if(UNIX AND NOT WIN32)
    find_program(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin)
    if(CMAKE_UNAME)
        exec_program(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
        set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL "processor type (i386 and x86_64)")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
            add_definitions(-fPIC)
        endif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    endif(CMAKE_UNAME)
endif(UNIX AND NOT WIN32)

enable_testing()

add_subdirectory(qdcm)
add_subdirectory(sklyph)
