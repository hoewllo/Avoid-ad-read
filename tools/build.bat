@echo off
setlocal enabledelayedexpansion

echo ========================================
echo EPUB广告清理工具 - Windows构建脚本
echo ========================================

REM 检查是否安装了CMake
where cmake >nul 2>nul
if errorlevel 1 (
    echo 错误: 未找到CMake，请先安装CMake
    echo 下载地址: https://cmake.org/download/
    pause
    exit /b 1
)

REM 检查是否安装了Visual Studio编译器
where cl >nul 2>nul
if errorlevel 1 (
    echo 警告: 未找到Visual Studio编译器
    echo 正在检查MinGW...
    where g++ >nul 2>nul
    if errorlevel 1 (
        echo 错误: 未找到C++编译器
        echo 请安装Visual Studio或MinGW
        pause
        exit /b 1
    ) else (
        set COMPILER_TYPE=MINGW
        echo 找到MinGW编译器
    )
) else (
    set COMPILER_TYPE=MSVC
    echo 找到Visual Studio编译器
)









REM 询问是否构建测试
set /p BUILD_TESTS="是否构建测试? (y/n): "
if /i "%BUILD_TESTS%"=="y" (
    set TEST_OPTION=-DBUILD_TESTS=ON
    echo 将构建测试
) else (


    set TEST_OPTION=-DBUILD_TESTS=OFF
    echo 不构建测试
)











REM 创建构建目录
if not exist "build" mkdir build
cd build

REM 运行CMake
if "%COMPILER_TYPE%"=="MSVC" (

    echo 使用Visual Studio生成解决方案...
    cmake .. -G "Visual Studio 17 2022" -A x64 %TEST_OPTION%
) else (

    echo 使用MinGW生成Makefile...
    cmake .. -G "MinGW Makefiles" %TEST_OPTION%
)

if errorlevel 1 (

    echo CMake配置失败
    cd ..
    pause
    exit /b 1
)










REM 编译项目
echo.
echo 正在编译项目...
if "%COMPILER_TYPE%"=="MSVC" (
    cmake --build . --config Release
) else (

    mingw32-make
)

if errorlevel 1 (
    echo 编译失败
    cd ..
    pause
    exit /b 1
)

REM 检查输出文件
if exist "bin\Release\epub_cleaner.exe" (
    copy "bin\Release\epub_cleaner.exe" "..\epub_cleaner.exe"
    echo.
    echo 编译成功! 可执行文件: epub_cleaner.exe
) else if exist "bin\epub_cleaner.exe" (
    copy "bin\epub_cleaner.exe" "..\epub_cleaner.exe"
    echo.
    echo 编译成功! 可执行文件: epub_cleaner.exe
) else (
    echo 警告: 未找到输出文件
)

REM 检查测试可执行文件
if /i "%BUILD_TESTS%"=="y" (
    if exist "bin\Release\test_epub_cleaner.exe" (
        copy "bin\Release\test_epub_cleaner.exe" "..\test_epub_cleaner.exe"
        echo 测试可执行文件: test_epub_cleaner.exe
    ) else if exist "bin\test_epub_cleaner.exe" (
        copy "bin\test_epub_cleaner.exe" "..\test_epub_cleaner.exe"
        echo 测试可执行文件: test_epub_cleaner.exe
    )
)

cd ..
echo.
echo ========================================
echo 构建完成!
echo ========================================
echo.
echo 使用方法:
echo   epub_cleaner -i 输入文件.epub -o 输出文件.epub
echo   epub_cleaner -I 输入目录 -O 输出目录
echo.
if /i "%BUILD_TESTS%"=="y" (
    echo 运行测试:
    echo   test_epub_cleaner
    echo.
)
pause
