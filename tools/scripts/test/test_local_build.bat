@echo off
echo ============================================
echo EPUB Ad Cleaner - 本地构建测试脚本
echo ============================================
echo.

REM 检查是否在项目根目录
if not exist "CMakeLists.txt" (
    echo 错误：请在项目根目录运行此脚本
    echo 当前目录：%cd%
    pause
    exit /b 1
)

echo 1. 清理之前的构建...
if exist "build" rmdir /s /q build
if exist "release" rmdir /s /q release

echo.
echo 2. 创建构建目录...
mkdir build

echo.
echo 3. 配置CMake...
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo CMake配置失败！
    cd ..
    pause
    exit /b 1
)

echo.
echo 4. 构建项目...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo 构建失败！
    cd ..
    pause
    exit /b 1
)

echo.
echo 5. 检查构建结果...
cd ..
if exist "build\bin\epub_cleaner.exe" (
    echo 构建成功！可执行文件位置：build\bin\epub_cleaner.exe
    echo.
    echo 6. 测试运行...
    echo 创建测试EPUB文件...
    echo 这是一个测试EPUB文件 > test.epub
    echo.
    echo 运行清理器...
    build\bin\epub_cleaner.exe test.epub test_clean.epub
    if %errorlevel% neq 0 (
        echo 程序运行失败（可能缺少依赖库）
    ) else (
        echo 程序运行成功！
    )
    del test.epub test_clean.epub 2>nul
) else (
    echo 错误：未找到可执行文件！
)

echo.
echo ============================================
echo 本地构建测试完成
echo ============================================
pause