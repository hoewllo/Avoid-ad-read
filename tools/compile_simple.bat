@echo off
setlocal enabledelayedexpansion

echo ========================================
echo EPUB广告清理工具 - 简单编译脚本
echo ========================================

REM 检查编译器
where g++ >nul 2>nul
if errorlevel 1 (
    echo 错误: 未找到g++编译器
    echo 请安装MinGW或配置PATH环境变量
    pause
    exit /b 1
)

echo 找到g++编译器

REM 创建输出目录
if not exist "bin" mkdir bin

REM 编译命令
set SOURCES=src\main.cpp src\epub_processor.cpp src\ad_patterns.cpp src\file_utils.cpp
set INCLUDES=-Iinclude -I.
set FLAGS=-std=c++17 -Wall -Wextra -O2

REM 检查是否在Windows上
if "%OS%"=="Windows_NT" (
    set FLAGS=!FLAGS! -D_WIN32
)

echo 正在编译...
g++ !FLAGS! !INCLUDES! !SOURCES! -o bin\epub_cleaner.exe

if errorlevel 1 (
    echo 编译失败
    pause
    exit /b 1
)

echo.
echo ========================================
echo 编译成功!
echo ========================================
echo 可执行文件: bin\epub_cleaner.exe
echo.
echo 使用方法:
echo   epub_cleaner -i 输入文件.epub -o 输出文件.epub
echo   epub_cleaner --help 查看所有选项
echo.
pause