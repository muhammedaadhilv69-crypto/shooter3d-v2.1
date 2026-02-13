@echo off
echo === Building Portable FPS Game for Windows ===
echo.

REM Check if SDL2 source exists
if not exist "external\SDL2" (
    echo SDL2 source not found. Please download it first.
    echo Run: download_sdl2_windows.bat
    echo.
    pause
    exit /b 1
)

REM Create build directory
if not exist build-portable mkdir build-portable
cd build-portable

echo.
echo Configuring build...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

if %errorlevel% neq 0 (
    echo.
    echo CMake configuration failed!
    pause
    exit /b %errorlevel%
)

echo.
echo Building (this will take a few minutes as SDL2 is compiled from source)...
mingw32-make -j4

if %errorlevel% neq 0 (
    echo.
    echo Build failed!
    pause
    exit /b %errorlevel%
)

echo.
echo === Build Complete! ===
echo.
echo Portable game location: build-portable\bin\
echo.
echo Files created:
dir /b bin
echo.
echo To run: cd build-portable\bin ^&^& FPSGame.exe
echo.
echo You can copy the entire 'bin' folder to any Windows machine!
echo SDL2.dll is included - no installation needed!
echo.
pause
