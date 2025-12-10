@echo off
REM filepath: build_tests.bat
if not exist ".\bin\test" mkdir ".\bin\test"

echo Compiling Unity tests...
gcc -c tests\unity.c -o bin\test\unity.o
gcc -c AvCalc.c -o bin\test\AvCalc.o
gcc -c tests\test_AvCalc.c -Itests -o bin\test\test_AvCalc.o

echo Linking...
gcc bin\test\unity.o bin\test\AvCalc.o bin\test\test_AvCalc.o -o bin\test\test_AvCalc.exe -lm

if %ERRORLEVEL% neq 0 (
    echo Build failed
    exit /b %ERRORLEVEL%
)

echo Running tests...
echo.
bin\test\test_AvCalc.exe
pause