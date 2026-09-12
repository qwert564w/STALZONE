@echo off
:: COPYRIGHT VV.EXE Development Team 2026
title VV.EXE Loader & Launcher
color 0A
echo ==========================================
echo    VV.EXE LAUNCHER - STALZONE
echo ==========================================
echo.
echo Please select an option:
echo.
echo [1] Start Game NORMAL (Will prompt for Admin/UAC)
echo [2] Start Game with UAC BYPASS (No Admin Prompt)
echo [3] Inject DLL into running game
echo [4] Exit
echo.
set /p choice="Enter choice (1-4): "

if "%choice%"=="1" goto normal_start
if "%choice%"=="2" goto bypass_start
if "%choice%"=="3" goto inject_dll
if "%choice%"=="4" exit

goto start

:normal_start
echo [*] Starting game normally...
:: Replace with actual Steam URL or path to game executable
start "" "steam://rungameid/YOUR_STEAM_APP_ID"
goto end

:bypass_start
echo [*] Launching game with UAC Bypass...
:: The UAC bypass tool will execute the game with High Integrity Level
:: Replace with actual path to game executable
tools\uac_bypass.exe "C:\Program Files (x86)\Steam\steamapps\common\STALZONE\game.exe"
goto end

:inject_dll
echo [*] Injecting vv.dll into Stalker.exe...
if not exist "build\Release\vv.dll" (
    echo [!] vv.dll not found! Run build.bat first.
    pause
    goto start
)
:: Using Extreme Injector via CLI
if exist "tools\ExtremeInjector.exe" (
    start "" "tools\ExtremeInjector.exe" -n "Stalker.exe" -d "build\Release\vv.dll" -i 2 -auto
) else (
    echo [!] ExtremeInjector.exe not found in tools/!
)
goto end

:end
echo [*] Done.
timeout /t 3 > nul
