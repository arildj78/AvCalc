@echo off
REM filepath: build_demo.bat
if not exist ".\bin" mkdir ".\bin"

echo Building AvCalc demo executable with debug symbols...
gcc -g AvCalc.c AvCalc_demo.c -o bin\AvCalc_demo.exe -lm

if %ERRORLEVEL% neq 0 (
    echo Build failed
    exit /b %ERRORLEVEL%
)

echo Build successful!
echo Running demo...
echo.
bin\AvCalc_demo.exe