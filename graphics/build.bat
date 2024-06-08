:: Main script
echo Running with administrator privileges...

:: Remove all contents in the build directory
if exist build (
    rmdir build
)

mkdir build

:: Run CMake to configure and build the project
cmake -S . -B build
cmake --build build
