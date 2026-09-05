cmake_minimum_required(VERSION 3.13)

project(randomdudeidk555.audio_cue_trainer VERSION 1.0.0 LANGUAGES CXX)

# Use a fixed C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# Make sure setup_geode_mod is available before calling it
if(NOT COMMAND setup_geode_mod)
    message(FATAL_ERROR "setup_geode_mod() not found. Ensure the Geode SDK/CMake modules are available in your CMake prefix or that you're using the Geode build workflow.")
endif()
setup_geode_mod(${PROJECT_NAME})
