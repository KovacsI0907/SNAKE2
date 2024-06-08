@echo off

REM Function to check if checksum has changed
:check_checksum
REM Define paths
set "FILE=CMakeLists.txt"
set "CHECKSUM_FILE=cmake.checksum"

REM Get current checksum of CMakeLists.txt
for /f %%i in ('CertUtil -hashfile "%FILE%" MD5 ^| findstr /v "hash"') do set "current_checksum=%%i"

REM Check if checksum.txt exists
if exist "%CHECKSUM_FILE%" (
    REM Read last checksum from checksum.txt
    set /p last_checksum=<"%CHECKSUM_FILE%"
    
    REM Compare current and last checksum
    if "%current_checksum%"=="%last_checksum%" (
        call :no_changes_detected
    ) else (
        REM Update checksum.txt with current checksum
        echo %current_checksum% > "%CHECKSUM_FILE%"
        call :checksum_changed
    )
) else (
    REM If checksum.txt doesn't exist, create it with current checksum
    echo %current_checksum% > "%CHECKSUM_FILE%"
    call :checksum_file_created
)
goto :eof

REM Function called when no changes are detected
:no_changes_detected
cmake --build ./build
exit /b 0

REM Function called when checksum has changed
:checksum_changed
rd /s /q build
mkdir build
cmake -S . -B ./build
cmake --build ./build
exit /b 1

REM Function called when checksum file is created
:checksum_file_created
rd /s /q build
mkdir build
cmake -S . -B ./build
cmake --build ./build
exit /b 1
