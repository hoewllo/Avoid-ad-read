@echo off
echo Collecting runtime libraries for EPUB Ad Cleaner
echo ================================================
echo.

REM 检查参数
if "%1"=="" (
    echo Usage: collect_runtime.bat <build_directory> [output_directory]
    echo Example: collect_runtime.bat build\msvc\bin dist\runtime
    exit /b 1
)

set BUILD_DIR=%1
set OUTPUT_DIR=%2

if "%OUTPUT_DIR%"=="" (
    set OUTPUT_DIR=dist\runtime
)

echo Build directory: %BUILD_DIR%
echo Output directory: %OUTPUT_DIR%
echo.

REM 创建输出目录
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

REM 复制可执行文件
echo Copying executables...
if exist "%BUILD_DIR%\*.exe" (
    copy "%BUILD_DIR%\*.exe" "%OUTPUT_DIR%\" >nul
    echo   Copied executables
)

REM 复制DLL文件
echo Copying DLLs...
if exist "%BUILD_DIR%\*.dll" (
    copy "%BUILD_DIR%\*.dll" "%OUTPUT_DIR%\" >nul
    echo   Copied DLLs from build directory
)

REM 对于MSVC构建，尝试收集VC++运行时
echo Checking for MSVC runtime libraries...
where vcpkg >nul 2>nul
if %errorlevel% equ 0 (
    echo   vcpkg found, checking for runtime DLLs...
    REM 这里可以添加vcpkg特定的运行时收集逻辑
)

REM 对于MinGW构建，尝试收集MinGW运行时
echo Checking for MinGW runtime libraries...
where gcc >nul 2>nul
if %errorlevel% equ 0 (
    echo   MinGW GCC found, checking for runtime DLLs...
    
    REM 查找MinGW运行时DLL
    for /f "tokens=*" %%i in ('where gcc') do (
        set GCC_PATH=%%~dpi
    )
    
    if defined GCC_PATH (
        echo   GCC path: %GCC_PATH%
        
        REM 复制常见的MinGW运行时DLL
        if exist "%GCC_PATH%libstdc++-6.dll" (
            copy "%GCC_PATH%libstdc++-6.dll" "%OUTPUT_DIR%\" >nul
            echo   Copied libstdc++-6.dll
        )
        
        if exist "%GCC_PATH%libgcc_s_seh-1.dll" (
            copy "%GCC_PATH%libgcc_s_seh-1.dll" "%OUTPUT_DIR%\" >nul
            echo   Copied libgcc_s_seh-1.dll
        )
        
        if exist "%GCC_PATH%libwinpthread-1.dll" (
            copy "%GCC_PATH%libwinpthread-1.dll" "%OUTPUT_DIR%\" >nul
            echo   Copied libwinpthread-1.dll
        )
    )
)

echo.
echo Runtime collection complete!
echo Files are in: %OUTPUT_DIR%
dir /b "%OUTPUT_DIR%"
echo.