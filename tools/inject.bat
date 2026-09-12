@echo off
:: COPYRIGHT VV.EXE Development Team 2026
echo [GORILLA] Building vv.dll via CMake...
if not exist "build" mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cd ..

echo [GORILLA] Checking binary for suspicious strings...
python tools/check_strings.py build/Release/vv.dll

echo [GORILLA] Launching Extreme Injector...
if exist "tools/ExtremeInjector.exe" (
    start "" "tools/ExtremeInjector.exe" -n "Stalker.exe -d "build/Release/vv.dll" -i 2 -auto
) else (
    echo ExtremeInjector.exe not found in tools/!
)
