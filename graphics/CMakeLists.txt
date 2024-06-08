cmake_minimum_required(VERSION 3.10)

# Set the project name
project(snakegraphics)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(src)
include_directories(src/module1)
include_directories(src/module2)

# Collect all source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/module1/*.cpp"
    "src/module2/*.cpp"
)

# Set the output directory for the binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add the executable
add_executable(snakegraphics ${SOURCES})
