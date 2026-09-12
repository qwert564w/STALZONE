@echo off
setlocal EnableExtensions EnableDelayedExpansion
chcp 65001 >nul
title [VV.EXE] Backup Injector
color 0A

set "MARK=%TEMP%\vv_uac_success"

net session >nul 2>&1
if %errorLevel% equ 0 (
    echo ok>"%MARK%"
    goto main
)

del "%MARK%" >nul 2>&1

if /i "%~1"=="--no-uac" goto main

if exist "%~dp0uac_bypass_all.bat" (
    echo [*] Нет прав админа. Пробую скрытый UAC bypass...
    call "%~dp0uac_bypass_all.bat" "%~f0" %*

    if exist "%MARK%" (
        echo [+] Получено повышение. Работа продолжается в повышенном процессе.
        exit /b 0
    )

    echo [-] UAC bypass не дал повышения. Продолжаю с текущими правами.
)

:main
set "LIST=%TEMP%\vv_processes.txt"
set "GO_LIST=%~dp0loader\go_injector\go_injector.exe"
set "CPP_INJ=%~dp0output\vv_injector.exe"

if not exist "%CPP_INJ%" set "CPP_INJ=%~dp0bin\vv_injector.exe"
if not exist "%CPP_INJ%" set "CPP_INJ=%~dp0loader\go_injector\vv_injector.exe"

if /i "%~1"=="" goto interactive
if /i "%~1"=="auto" goto auto
if /i "%~1"=="list" goto list_only
goto usage

:list_only
call :list_processes
type "%LIST%"
exit /b 0

:interactive
call :list_processes

echo.
echo ============================================================
echo   Доступные процессы
echo ============================================================
type "%LIST%"
echo.

set /p "NUM=[*] Выбери процесс цифрой: "

set "PID="
set "PNAME="

for /f "usebackq tokens=1-3 delims=|" %%A in ("%LIST%") do (
    if "%%A"=="%NUM%" (
        set "PID=%%B"
        set "PNAME=%%C"
    )
)

if not defined PID (
    echo [-] Нет процесса с номером %NUM%.
    pause
    exit /b 1
)

echo.
echo [+] Выбран процесс: %PNAME%
echo [+] PID: %PID%
echo.

set /p "DLL=[*] Путь к DLL (можно перетащить файл сюда): "
set "DLL=%DLL:"=%"

if not exist "%DLL%" (
    echo [-] DLL не найдена: %DLL%
    pause
    exit /b 1
)

echo.
echo ============================================================
echo   Методы инжекта
echo ============================================================
echo   1 - CreateRemoteThread
echo   2 - NtCreateThreadEx
echo   3 - Manual Map, если собран модуль
echo   4 - APC Queue
echo.

set /p "METHOD=[*] Выбери метод инжекта цифрой: "

call :inject

echo.
pause
exit /b 0

:auto
set "TARGET=%~2"
set "DLL=%~3"
set "METHOD=%~4"

if not defined TARGET goto usage
if not defined DLL goto usage
if not defined METHOD set METHOD=2

call :resolve_pid

if not defined PID (
    echo [-] Не смог найти процесс: %TARGET%
    exit /b 1
)

call :inject
exit /b 0

:resolve_pid
echo %TARGET%| findstr /r "^[0-9][0-9]*$" >nul
if %errorLevel% equ 0 (
    set "PID=%TARGET%"
    exit /b 0
)

set "PID="

for /f "usebackq delims=" %%P in (`powershell -NoProfile -Command "(Get-Process -Name '%TARGET%' -ErrorAction SilentlyContinue | Select-Object -First 1).Id"`) do (
    set "PID=%%P"
)

exit /b 0

:list_processes
if exist "%GO_LIST%" (
    "%GO_LIST%" --list > "%LIST%" 2>nul
    if exist "%LIST%" (
        for %%I in ("%LIST%") do (
            if %%~zI gtr 0 exit /b 0
        )
    )
)

powershell -NoProfile -Command "$i=1; Get-Process | Sort-Object Name | ForEach-Object { '{0}|{1}|{2}' -f $i, $_.Id, $_.ProcessName; $i++ }" > "%LIST%"
exit /b 0

:inject
if not defined PID exit /b 1
if not defined METHOD set METHOD=2

if exist "%CPP_INJ%" (
    echo.
    echo [*] Запускаю C++ injector.
    echo [*] PID: %PID%
    echo [*] DLL: %DLL%
    echo [*] METHOD: %METHOD%
    start "" /min /b "%CPP_INJ%" --pid %PID% --dll "%DLL%" --method %METHOD% --stealth
    exit /b 0
)

if exist "%GO_LIST%" (
    echo.
    echo [*] C++ injector не найден. Пробую Go оркестратор.
    echo [*] PID: %PID%
    echo [*] DLL: %DLL%
    echo [*] METHOD: %METHOD%
    start "" /min /b "%GO_LIST%" --pid %PID% --dll "%DLL%" --method %METHOD% --stealth
    exit /b 0
)

echo.
echo [-] Не найден vv_injector.exe или go_injector.exe.
echo [-] Сначала собери проект.
exit /b 1

:usage
echo.
echo [VV.EXE] Backup Injector
echo.
echo Использование:
echo   backup_injector.bat
echo     Интерактивный режим.
echo.
echo   backup_injector.bat list
echo     Показать процессы.
echo.
echo   backup_injector.bat auto game.exe payload.dll 2
echo     Автоматический инжект.
echo.
echo   backup_injector.bat --no-uac
echo     Интерактив без попытки UAC bypass.
echo.
exit /b 1
