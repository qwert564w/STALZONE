@echo off
setlocal EnableDelayedExpansion
title VV.EXE Auto Setup - No Admin Required
color 0A

echo ======================================
echo   VV.EXE AUTO SETUP (NO ADMIN)
echo   COPYRIGHT VV.EXE Development Team 2026
echo ======================================
echo.

if not exist "tools" mkdir tools
if not exist "libs" mkdir libs
if not exist "build" mkdir build
if not exist "output" mkdir output

echo [1/6] Checking for Visual Studio Build Tools...
set "VS_PATH="
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
    set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
    set "VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
)

if "!VS_PATH!"=="" (
    echo [!] Visual Studio Build Tools not found!
    echo [!] Please install Visual Studio Build Tools from: https://aka.ms/vs/17/release/vs_BuildTools.exe
    echo [!] Select "Desktop development with C++"
    pause
    exit /b 1
)

echo [OK] Found VS at: !VS_PATH!
call "!VS_PATH!"

echo.
echo [2/6] Downloading CMake (Portable, No Admin)...
if not exist "tools\cmake\bin\cmake.exe" (
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1-windows-x64.zip' -OutFile 'tools\cmake.zip'"
    powershell -Command "Expand-Archive -Path 'tools\cmake.zip' -DestinationPath 'tools\cmake_temp' -Force"
    move "tools\cmake_temp\cmake-3.28.1-windows-x64" "tools\cmake" >nul
    del "tools\cmake.zip" >nul
    rmdir /s /q "tools\cmake_temp" >nul
)
echo [OK] CMake ready.

echo.
echo [3/6] Downloading MinHook (for DX hook)...
if not exist "libs\minhook\include\MinHook.h" (
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/TsudaKageyu/minhook/archive/refs/heads/master.zip' -OutFile 'libs\minhook.zip'"
    powershell -Command "Expand-Archive -Path 'libs\minhook.zip' -DestinationPath 'libs\minhook_temp' -Force"
    move "libs\minhook_temp\minhook-master" "libs\minhook" >nul
    del "libs\minhook.zip" >nul
    rmdir /s /q "libs\minhook_temp" >nul
)
echo [OK] MinHook ready.

echo.
echo [4/6] Downloading ImGui (for Menu)...
if not exist "libs\imgui\imgui.h" (
    mkdir libs\imgui >nul
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/imgui.h' -OutFile 'libs\imgui\imgui.h'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/imgui.cpp' -OutFile 'libs\imgui\imgui.cpp'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/imgui_draw.cpp' -OutFile 'libs\imgui\imgui_draw.cpp'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/imgui_tables.cpp' -OutFile 'libs\imgui\imgui_tables.cpp'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/imgui_widgets.cpp' -OutFile 'libs\imgui\imgui_widgets.cpp'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_dx11.h' -OutFile 'libs\imgui\imgui_impl_dx11.h'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_dx11.cpp' -OutFile 'libs\imgui\imgui_impl_dx11.cpp'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_win32.h' -OutFile 'libs\imgui\imgui_impl_win32.h'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_win32.cpp' -OutFile 'libs\imgui\imgui_impl_win32.cpp'"
)
echo [OK] ImGui ready.

echo.
echo [5/6] Downloading Il2CppDumper (just in case)...
if not exist "tools\Il2CppDumper.exe" (
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/Perfare/Il2CppDumper/releases/download/v6.7.46/Il2CppDumper-v6.7.46.zip' -OutFile 'tools\Il2CppDumper.zip'"
    powershell -Command "Expand-Archive -Path 'tools\Il2CppDumper.zip' -DestinationPath 'tools\Il2CppDumper_temp' -Force"
    move "tools\Il2CppDumper_temp\*" "tools\" >nul
    del "tools\Il2CppDumper.zip" >nul
    rmdir /s /q "tools\Il2CppDumper_temp" >nul
)
echo [OK] Il2CppDumper ready.

echo.
echo [6/6] Compiling DLL...
cd build
set "TOOLS_DIR=%CD%\..\tools"
"!TOOLS_DIR!\cmake\bin\cmake.exe" .. -G "Visual Studio 17 2022" -A x64
"!TOOLS_DIR!\cmake\bin\cmake.exe" --build . --config Release
if !errorlevel! equ 0 (
    copy /y "Release\vv.dll" "..\output\vv.dll" >nul
    echo.
    echo ======================================
    echo   [SUCCESS] vv.dll compiled!
    echo   Location: output\vv.dll
    echo ======================================
    echo.
    echo Now use Extreme Injector v3 to inject output\vv.dll into the game!
) else (
    echo [ERROR] Compilation failed! Check errors above.
)
cd ..
pause
