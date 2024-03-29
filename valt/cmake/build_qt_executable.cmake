if(UNIX)
    add_definitions(-Wall)
endif()

# Look for Qt libraries
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DQT_NO_DEBUG)
endif(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
add_definitions(-DQT_FATAL_ASSERT)

# Add dependent includes
include(add_includes)

# Wrap Qt meta-objects
set(META_SRCS ${MOCS})
qt4_wrap_cpp(MOC_SRCS ${META_SRCS})

# Wrap UI
qt4_wrap_ui(UI_SRCS ${UIS})

# Resources
qt4_add_resources(RES_SRCS ${RES})

# Translations
qt4_add_translation(TRANSLATIONS ${TSS})

# Add OPENGL includes
include(add_opengl)

set(GCOV "")
if(UNIX AND QDCM_TESTS_COVERAGE)
    set(GCOV "gcov")
endif()

if(${PROJECT_NAME}_DISABLE_CONSOLE)
    if(WIN32)
        set(${PROJECT_NAME}_GUI WIN32)
        set(${PROJECT_NAME}_QT_MAIN ${QT_QTMAIN_LIBRARY})
    endif()
endif()

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_GUI} ${SRCS} ${MOC_SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${HDRS})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_QT_MAIN} ${QT_LIBRARIES} ${LIBS} ${GCOV})

# Link OPENGL libraries
include(link_opengl)

install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION ${INSTALL_BIN}
        COMPONENT runtime)

if(MSVC)
    if(${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
        install(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.pdb"
                DESTINATION ${INSTALL_BIN}
                COMPONENT runtime)
    endif()
endif()
