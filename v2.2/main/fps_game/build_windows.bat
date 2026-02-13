@echo off
echo === Building FPS Game for Windows ===

if not exist build mkdir build
cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b %errorlevel%
)

mingw32-make -j4
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)

echo.
echo === Build Complete ===
echo Executable: build\bin\FPSGame.exe
echo.
pause
