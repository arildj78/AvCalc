@echo off
REM filepath: build_dll.bat
if not exist ".\bin" mkdir ".\bin"

echo Building AvCalc.dll...
gcc -D BUILD_DLL -shared -o bin\AvCalc.dll AvCalc.c -lm

if %ERRORLEVEL% neq 0 (
    echo DLL build failed with error code %ERRORLEVEL%.
    echo Check that gcc is installed and AvCalc.c exists.
    exit /b %ERRORLEVEL%
)

echo DLL build successful!
pause