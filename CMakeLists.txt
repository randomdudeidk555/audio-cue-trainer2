cmake_minimum_required(VERSION 3.21)

project(AudioCueTrainer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR
        "GEODE_SDK is not set. Please install/setup the Geode SDK."
    )
endif()

message(STATUS "Found Geode SDK: $ENV{GEODE_SDK}")

add_subdirectory(
    $ENV{GEODE_SDK}
    ${CMAKE_CURRENT_BINARY_DIR}/geode
)

setup_geode_mod(${PROJECT_NAME}
    SOURCES ${SOURCES}
)
