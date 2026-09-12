@echo off
echo ======================================
echo   VV.EXE Game Launcher
echo   COPYRIGHT VV.EXE Development Team 2026
echo ======================================
echo.

:menu
cls
echo ======================================
echo   Выберите действие:
echo ======================================
echo.
echo   1. Запустить игру нормально (с UAC)
echo   2. Запустить с UAC BYPASS
echo   3. Инжект в запущенную игру
echo   4. Выход
echo.
set /p choice="Ваш выбор: "

if "%choice%"=="1" goto normal_start
if "%choice%"=="2" goto uac_bypass
if "%choice%"=="3" goto inject
if "%choice%"=="4" goto exit
goto menu

:normal_start
echo.
echo [LAUNCHER] Запуск игры нормально...
if not exist "C:\Games\STALZONE\game.exe" (
    echo [LAUNCHER] ERROR: Game not found at C:\Games\STALZONE\game.exe
    pause
    goto menu
)
start "" "C:\Games\STALZONE\game.exe"
echo [LAUNCHER] ✓ Игра запущена!
timeout /t 3 /nobreak >nul
goto menu

:uac_bypass
echo.
echo [LAUNCHER] Запуск с UAC BYPASS...
if not exist "build\uac_bypass_v2.exe" (
    echo [LAUNCHER] ERROR: UAC Bypass not found! Run build.bat first.
    pause
    goto menu
)
if not exist "C:\Games\STALZONE\game.exe" (
    echo [LAUNCHER] ERROR: Game not found at C:\Games\STALZONE\game.exe
    pause
    goto menu
)
build\uac_bypass_v2.exe "C:\Games\STALZONE\game.exe"
if %errorlevel% equ 0 (
    echo [LAUNCHER] ✓ Игра запущена с правами админа!
) else (
    echo [LAUNCHER] ✗ Ошибка UAC Bypass!
)
timeout /t 3 /nobreak >nul
goto menu

:inject
echo.
echo [LAUNCHER] Инжект в запущенную игру...
if not exist "vv.dll" (
    echo [LAUNCHER] ERROR: vv.dll not found! Run build.bat first.
    pause
    goto menu
)

:: Проверяем что игра запущена
tasklist /FI "IMAGENAME eq game.exe" | find /I "game.exe" >nul
if %errorlevel% neq 0 (
    echo [LAUNCHER] ERROR: Game is not running! Start the game first.
    pause
    goto menu
)

:: Запускаем loader для инжекта
if exist "loader\loader.exe" (
    loader\loader.exe
) else (
    echo [LAUNCHER] WARNING: loader.exe not found. Trying manual inject...
    :: Здесь можно добавить ручной инжект через CreateRemoteThread
    echo [LAUNCHER] Manual inject not implemented yet.
)
timeout /t 3 /nobreak >nul
goto menu

:exit
echo.
echo [LAUNCHER] Выход...
timeout /t 1 /nobreak >nul
exit /b 0
