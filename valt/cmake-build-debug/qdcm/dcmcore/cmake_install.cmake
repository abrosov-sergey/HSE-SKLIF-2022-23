# Install script for directory: /Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmcore" TYPE FILE FILES
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmCharSet.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmCoreApi.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmDataset.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmDictionary.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmFile.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmGlobal.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmModule.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmPDUItem.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmReader.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmStream.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTag.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagAE.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagAS.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagAT.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagBinary.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagCS.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagDA.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagDS.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagDT.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagDescription.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagFD.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagFL.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagGroup.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagIS.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagItem.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagKey.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagLO.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagLT.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagList.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagPN.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagPixelData.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagPixelDataItem.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagSH.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagSL.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagSQ.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagSS.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagST.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagString.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagTM.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagUI.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagUL.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagUS.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTagUT.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTransferSyntax.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmTypes.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmVr.h"
    "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/qdcm/dcmcore/include/DcmWriter.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/deploy/debug/libdcmcore.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmcore.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmcore.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libdcmcore.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmcore.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmcore.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/sergeyabrosov/Desktop/Project/sklif-2022-23/valt/deploy/debug/libdcmcore.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmcore.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmcore.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libdcmcore.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmcore.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdcmcore.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

