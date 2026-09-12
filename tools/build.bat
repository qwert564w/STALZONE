@echo off
echo ======================================
echo   VV.EXE Build Script
echo   COPYRIGHT VV.EXE Development Team 2026
echo ======================================
echo.

echo [BUILD] Starting build process...
echo.

:: Проверяем наличие CMake
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [BUILD] ERROR: CMake not found! Please install CMake.
    pause
    exit /b 1
)

:: Проверяем наличие cl.exe
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo [BUILD] ERROR: cl.exe not found! Please run from Visual Studio Developer Command Prompt.
    pause
    exit /b 1
)

:: Создаём директорию build
if not exist build mkdir build
cd build

:: Запускаем CMake
echo [BUILD] Running CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if %errorlevel% neq 0 (
    echo [BUILD] ERROR: CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

:: Компилируем проект
echo [BUILD] Building project...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo [BUILD] ERROR: Build failed!
    cd ..
    pause
    exit /b 1
)

echo [BUILD] ✓ vv.dll compiled successfully!
echo.

:: Копируем DLL в корневую директорию
copy /y Release\vv.dll ..\vv.dll >nul
echo [BUILD] ✓ vv.dll copied to root directory
echo.

:: Компилируем UAC Bypass
echo [BUILD] Compiling UAC Bypass v2...
cd ..
cl /EHsc /O2 tools\uac_bypass_v2.cpp /link advapi32.lib shell32.lib shlwapi.lib /Fe:build\uac_bypass_v2.exe
if %errorlevel% neq 0 (
    echo [BUILD] WARNING: UAC Bypass compilation failed!
) else (
    echo [BUILD] ✓ uac_bypass_v2.exe compiled successfully!
)
echo.

:: Проверяем строки на детект
echo [BUILD] Running string checker...
python tools\check_strings.py vv.dll
if %errorlevel% neq 0 (
    echo [BUILD] WARNING: Suspicious strings found in DLL!
) else (
    echo [BUILD] ✓ No suspicious strings found
)
echo.

echo ======================================
echo   Build completed successfully!
echo ======================================
echo.
echo Files created:
echo   - vv.dll (main cheat DLL)
echo   - uac_bypass_v2.exe (UAC bypass tool)
echo.
pause
