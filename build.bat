@echo off
echo Building VV_EXE.dll...
cmake -B build -A x64
cmake --build build --config Release
echo Done!
