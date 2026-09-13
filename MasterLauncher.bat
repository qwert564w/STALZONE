@echo off
setlocal EnableDelayedExpansion
title STALZONE MASTER LAUNCHER
color 0A

set "TOOLS_DIR=%~dp0tools"
set "LIBS_DIR=%~dp0libs"
set "BUILD_DIR=%~dp0build"
set "OUTPUT_DIR=%~dp0output"
set "BIN_DIR=%~dp0bin"
set "LOADER_DIR=%~dp0loader\go_injector"

for %%D in ("%TOOLS_DIR%" "%LIBS_DIR%" "%BUILD_DIR%" "%OUTPUT_DIR%" "%BIN_DIR%") do if not exist %%D mkdir %%D

echo [*] AUTO-DOWNLOADING DEPENDENCIES
if not exist "%TOOLS_DIR%\cmakein\cmake.exe" (
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1-windows-x64.zip' -OutFile '%TOOLS_DIR%\cmake.zip'"
    powershell -Command "Expand-Archive -Path '%TOOLS_DIR%\cmake.zip' -DestinationPath '%TOOLS_DIR%\cmake_temp' -Force"
    move "%TOOLS_DIR%\cmake_temp\cmake-3.28.1-windows-x64" "%TOOLS_DIR%\cmake" >nul
    del "%TOOLS_DIR%\cmake.zip" >nul
    rmdir /s /q "%TOOLS_DIR%\cmake_temp" >nul
)
if not exist "%TOOLS_DIR%\goin\go.exe" (
    powershell -Command "Invoke-WebRequest -Uri 'https://go.dev/dl/go1.21.5.windows-amd64.zip' -OutFile '%TOOLS_DIR%\go.zip'"
    powershell -Command "Expand-Archive -Path '%TOOLS_DIR%\go.zip' -DestinationPath '%TOOLS_DIR%' -Force"
    del "%TOOLS_DIR%\go.zip" >nul
)
set "PATH=%TOOLS_DIR%\goin;%PATH%"
if not exist "%LIBS_DIR%\minhook\include\MinHook.h" (
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/TsudaKageyu/minhook/archive/refs/heads/master.zip' -OutFile '%LIBS_DIR%\minhook.zip'"
    powershell -Command "Expand-Archive -Path '%LIBS_DIR%\minhook.zip' -DestinationPath '%LIBS_DIR%\minhook_temp' -Force"
    move "%LIBS_DIR%\minhook_temp\minhook-master" "%LIBS_DIR%\minhook" >nul
    del "%LIBS_DIR%\minhook.zip" >nul
)
if not exist "%LIBS_DIR%\imgui\imgui.h" (
    mkdir "%LIBS_DIR%\imgui" >nul
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/imgui.h' -OutFile '%LIBS_DIR%\imgui\imgui.h'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/imgui.cpp' -OutFile '%LIBS_DIR%\imgui\imgui.cpp'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/imgui_draw.cpp' -OutFile '%LIBS_DIR%\imgui\imgui_draw.cpp'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_dx11.h' -OutFile '%LIBS_DIR%\imgui\imgui_impl_dx11.h'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_dx11.cpp' -OutFile '%LIBS_DIR%\imgui\imgui_impl_dx11.cpp'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_win32.h' -OutFile '%LIBS_DIR%\imgui\imgui_impl_win32.h'"
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_win32.cpp' -OutFile '%LIBS_DIR%\imgui\imgui_impl_win32.cpp'"
)
if not exist "%TOOLS_DIR%\ExtremeInjector.exe" (
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/XMotoR/ExtremeInjector/releases/download/v3.0/Extreme.Injector.v3.0.-.XMotoR.exe' -OutFile '%TOOLS_DIR%\ExtremeInjector.exe'"
)

echo [*] COMPILING PROJECT
if not exist "%OUTPUT_DIR%v.dll" (
    set "VS_PATH="
    if exist "C:\Program Files\Microsoft Visual Studio2\Community\VC\Auxiliary\Buildcvars64.bat" set "VS_PATH=C:\Program Files\Microsoft Visual Studio2\Community\VC\Auxiliary\Buildcvars64.bat"
    if exist "C:\Program Files\Microsoft Visual Studio2\Professional\VC\Auxiliary\Buildcvars64.bat" set "VS_PATH=C:\Program Files\Microsoft Visual Studio2\Professional\VC\Auxiliary\Buildcvars64.bat"
    if exist "C:\Program Files\Microsoft Visual Studio2\Enterprise\VC\Auxiliary\Buildcvars64.bat" set "VS_PATH=C:\Program Files\Microsoft Visual Studio2\Enterprise\VC\Auxiliary\Buildcvars64.bat"
    if exist "C:\Program Files (x86)\Microsoft Visual Studio9\Community\VC\Auxiliary\Buildcvars64.bat" set "VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio9\Community\VC\Auxiliary\Buildcvars64.bat"
    if not "!VS_PATH!"=="" call "!VS_PATH!" >nul
    cd "%BUILD_DIR%"
    "%TOOLS_DIR%\cmakein\cmake.exe" .. -G "Visual Studio 17 2022" -A x64 >nul 2>&1
    "%TOOLS_DIR%\cmakein\cmake.exe" --build . --config Release >nul 2>&1
    if exist "Releasev.dll" copy /y "Releasev.dll" "%OUTPUT_DIR%v.dll" >nul
    cd ..
)
if not exist "%BIN_DIR%\go_injector.exe" (
    cd "%LOADER_DIR%"
    "%TOOLS_DIR%\goin\go.exe" build -o "%BIN_DIR%\go_injector.exe" main.go
    cd "%~dp0"
)

:MENU
cls
echo 1. UAC Bypass (Auto Launch + Inject)
echo 2. Normal Launch (Choose Injector)
echo 3. Exit
choice /c 123 /n /m "Select option: "
if errorlevel 3 exit /b
if errorlevel 2 goto NORMAL_LAUNCH
if errorlevel 1 goto UAC_BYPASS

:UAC_BYPASS
set "GAME_EXE=C:\Program Files (x86)\Steam\steamapps\common\STALZONE\STALKER.exe"
for /f "tokens=2*" %%A in ('reg query "HKCU\Software\Valve\Steam" /v SteamPath 2^>nul') do set "STEAM_PATH=%%B"
if defined STEAM_PATH set "GAME_EXE=!STEAM_PATH!\steamapps\common\STALZONE\STALKER.exe"
reg add "HKCU\Software\Classes\ms-settings\Shell\Open\command" /ve /d ""!GAME_EXE!"" /f >nul 2>&1
reg add "HKCU\Software\Classes\ms-settings\Shell\Open\command" /v "DelegateExecute" /f >nul 2>&1
start "" "C:\Windows\System32\computerdefaults.exe"
timeout /t 3 /nobreak >nul
reg delete "HKCU\Software\Classes\ms-settings" /f >nul 2>&1
timeout /t 10 /nobreak >nul
"%BIN_DIR%\go_injector.exe" --list > "%TEMP%\procs.txt"
set COUNT=0
for /f "tokens=1,2,3 delims=|" %%A in (%TEMP%\procs.txt) do (
    if !COUNT! LEQ 50 ( echo [%%A] PID: %%B - %%C & set /a COUNT+=1 )
)
set /p "TARGET_PID=Enter PID to inject into: "
"%BIN_DIR%\go_injector.exe" --pid !TARGET_PID! --dll "%OUTPUT_DIR%v.dll" --method 2 --stealth
pause
goto MENU

:NORMAL_LAUNCH
cls
echo 1. Use Go Injector (Stealth)
echo 2. Use Extreme Injector v3
echo 3. Back
choice /c 123 /n /m "Select option: "
if errorlevel 3 goto MENU
if errorlevel 2 goto EXTREME_INJECT
if errorlevel 1 goto GO_INJECT

:GO_INJECT
"%BIN_DIR%\go_injector.exe" --list
set /p "TARGET=Enter PID or Process Name: "
"%BIN_DIR%\go_injector.exe" --pid %TARGET% --dll "%OUTPUT_DIR%v.dll" --method 2 --stealth
pause
goto MENU

:EXTREME_INJECT
start "" "%TOOLS_DIR%\ExtremeInjector.exe"
pause
goto MENU
