@echo off
setlocal EnableDelayedExpansion
chcp 65001 >nul
title STALZONE - Master Loader
color 0A
if exist "%~dp0bin\dev_lock.key" (
    set /p KEY_CONTENT=<"%~dp0bin\dev_lock.key"
    if "!KEY_CONTENT!"=="STALZONE_MASTER_LOCK_9921" (
        color 0C
        echo [X] SYSTEM LOCKED
        pause >nul
        exit /b
    )
)
echo [*] STALZONE Loaded. LPE & Inject ready.
