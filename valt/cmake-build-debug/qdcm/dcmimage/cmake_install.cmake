# Install script for directory: /Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmimage

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xincludesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmimage" TYPE FILE FILES
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmimage/include/DcmImage.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmimage/include/DcmImageApi.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmimage/include/DcmImageTransferFunction.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmimage/include/DcmMonochromeImage.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmimage/include/DcmPhotometricInterpretation.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/deploy/debug/libdcmimage.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmimage.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmimage.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libdcmimage.dylib"
      -change "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/deploy/debug/libdcmcore.dylib" "libdcmcore.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmimage.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmimage.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/deploy/debug/libdcmimage.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmimage.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmimage.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libdcmimage.dylib"
      -change "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/deploy/debug/libdcmcore.dylib" "libdcmcore.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmimage.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmimage.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

