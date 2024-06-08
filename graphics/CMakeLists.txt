cmake_minimum_required(VERSION 3.14)

project(snakegraphics)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

# GOOGLE TEST
enable_testing()

# Collect all test source files
file(GLOB_RECURSE TEST_SOURCES
    "src/tests/*.cc"
)

# Add the test executable
add_executable(all_tests ${TEST_SOURCES})

# Link the test executable against GoogleTest
target_link_libraries(all_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(all_tests)