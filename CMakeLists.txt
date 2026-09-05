cmake_minimum_required(VERSION 3.13)

project(randomdudeidk555.audio_cue_trainer VERSION 1.0.0 LANGUAGES CXX)

# Add Geode SDK to CMake module path
list(APPEND CMAKE_PREFIX_PATH "$ENV{GEODE_SDK_PATH}")
list(APPEND CMAKE_MODULE_PATH "$ENV{GEODE_SDK_PATH}/cmake")
# Use a fixed C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add Geode SDK to CMake module path
# Adjust the path based on where your Geode SDK is installed
find_package(geode-sdk REQUIRED)

# Find source files (don't glob headers as compile sources)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")

# Fail early if no sources found
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in ${CMAKE_CURRENT_SOURCE_DIR}/src. Make sure the 'src' directory exists and contains .cpp/.cxx/.cc files.")
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Expose your headers (adjust path if headers are elsewhere)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# setup_geode_mod is now available after find_package(geode-sdk)
setup_geode_mod(${PROJECT_NAME})
