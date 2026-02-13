@echo off
echo === Downloading SDL2 Source for Portable Windows Build ===
echo.

set SDL2_VERSION=2.28.5
set SDL2_URL=https://github.com/libsdl-org/SDL/releases/download/release-%SDL2_VERSION%/SDL2-%SDL2_VERSION%.tar.gz

cd external

if not exist "SDL2" (
    echo Downloading SDL2 %SDL2_VERSION%...
    echo.
    
    REM Download using PowerShell
    powershell -Command "& {Invoke-WebRequest -Uri '%SDL2_URL%' -OutFile 'SDL2.tar.gz'}"
    
    if %errorlevel% neq 0 (
        echo Download failed!
        echo Please download manually from:
        echo %SDL2_URL%
        echo And extract to external\SDL2
        pause
        exit /b 1
    )
    
    echo Extracting SDL2...
    tar -xzf SDL2.tar.gz
    ren SDL2-%SDL2_VERSION% SDL2
    del SDL2.tar.gz
    
    echo SDL2 downloaded and extracted successfully!
) else (
    echo SDL2 already exists in external\SDL2
)

cd ..

echo.
echo === SDL2 Setup Complete ===
echo You can now build with: build_portable_windows.bat
echo.
pause
