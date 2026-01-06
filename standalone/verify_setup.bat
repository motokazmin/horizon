@echo off
echo ========================================
echo Horizon UTM - Prerequisites Verification
echo ========================================
echo.

echo [1/5] Checking Visual Studio...
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Visual Studio C++ compiler found
    cl 2>&1 | findstr "Version"
) else (
    echo [ERROR] Visual Studio compiler not found
    echo Please run this from "Developer Command Prompt for VS"
)
echo.

echo [2/5] Checking CMake...
where cmake >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] CMake found
    cmake --version | findstr "version"
) else (
    echo [ERROR] CMake not found in PATH
)
echo.

echo [3/5] Checking Qt...
where qmake >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Qt found
    qmake --version
) else (
    echo [ERROR] Qt not found in PATH
    echo Add Qt bin directory to PATH: C:\Qt\6.5.3\msvc2019_64\bin
)
echo.

echo [4/5] Checking Git (optional)...
where git >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Git found
    git --version
) else (
    echo [SKIP] Git not found (optional)
)
echo.

echo [5/5] Checking disk space...
echo [INFO] Make sure you have at least 10GB free on C:\
echo.

echo ========================================
echo Verification complete!
echo If all checks passed, you're ready to build.
echo ========================================
echo.
echo Next steps:
echo 1. mkdir build
echo 2. cd build
echo 3. cmake ..
echo 4. cmake --build . --config Release
echo.
pause
