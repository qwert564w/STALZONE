@echo off
setlocal EnableDelayedExpansion
title VV.EXE UAC Bypass Launcher
color 0B

echo ======================================
echo   VV.EXE UAC BYPASS LAUNCHER
echo   Bypassing UAC without admin password
echo ======================================
echo.

set "GAME_EXE=C:\Program Files (x86)\Steam\steamapps\common\STALZONE\STALKER.exe"
set "STEAM_APPID=1234567"

echo [INFO] Game path: !GAME_EXE!
echo.

echo [BYPASS] Trying method 1: computerdefaults.exe...
reg add "HKCU\Software\Classes\ms-settings\Shell\Open\command" /ve /d "\"!GAME_EXE!\"" /f >nul 2>&1
reg add "HKCU\Software\Classes\ms-settings\Shell\Open\command" /v "DelegateExecute" /f >nul 2>&1

start "" "C:\Windows\System32\computerdefaults.exe"
timeout /t 3 /nobreak >nul

reg delete "HKCU\Software\Classes\ms-settings" /f >nul 2>&1

echo [OK] Game launched with elevated rights (no UAC prompt)!
echo.
echo ======================================
echo   NOW INJECT:
echo   1. Wait for game to reach main menu
echo   2. Open Extreme Injector v3
echo   3. Select game process
echo   4. Add output\vv.dll
echo   5. Click INJECT
echo ======================================
pause
