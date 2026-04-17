@echo off
echo Testing EPUB Ad Cleaner build with fixes...
echo.

REM 清理之前的构建
if exist build rmdir /s /q build

REM 创建构建目录
mkdir build
cd build

REM 配置CMake
echo Configuring CMake...
cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    exit /b 1
)

REM 构建项目
echo Building project...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build failed!
    exit /b 1
)

REM 运行测试
echo Running tests...
ctest --output-on-failure
if %errorlevel% neq 0 (
    echo Tests failed!
    exit /b 1
)

echo.
echo Build and test completed successfully!
echo Executable location: build\bin\Release\epub_cleaner.exe
echo Test executable: build\bin\Release\test_epub_cleaner.exe

cd ..
pause