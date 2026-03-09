@echo off
setlocal

set BUILD_DIR=build

if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

cd "%BUILD_DIR%"

cmake -G "MinGW Makefiles" ..
if errorlevel 1 (
    echo cmake configuration failed
    exit /b 1
)

cmake --build . --config Release
if errorlevel 1 (
    echo build failed
    exit /b 1
)

echo build finished. executable should be in the build directory.

endlocal

