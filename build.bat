gcc -c -o AvCalc.o AvCalc.c -D AVCALC_EXPORTS
gcc -o .\bin\Release\AvCalc.dll AvCalc.o -shared -Wl,--subsystem,windows
del AvCalc.o