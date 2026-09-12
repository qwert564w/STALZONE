@echo off
:: COPYRIGHT VV.EXE Development Team 2026
echo ==========================================
echo    VV.EXE BUILD SYSTEM - STALZONE
echo ==========================================

echo [1/3] Building main cheat DLL (vv.dll)...
if not exist "build" mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cd ..

if not exist "build\Release\vv.dll" (
    echo [!] Failed to build vv.dll. Check CMake errors.
    pause
    exit /b 1
)

echo [2/3] Building UAC Bypass tool...
:: Compile uac_bypass.cpp using cl.exe (Visual Studio Compiler)
:: Ensure you run this from a "x64 Native Tools Command Prompt for VS 2022"
cl /EHsc /O2 tools\uac_bypass.cpp /Fe:tools\uac_bypass.exe /link user32.lib advapi32.lib shell32.lib > nul 2>&1
if not exist "tools\uac_bypass.exe" (
    echo [!] Warning: Failed to compile uac_bypass.exe. You might need to compile it manually in Visual Studio.
)

echo [3/3] Running string checks on vv.dll...
python tools\check_strings.py build\Release\vv.dll

echo ==========================================
echo    BUILD COMPLETE!
echo    Output: build\Release\vv.dll
echo ==========================================
pause
