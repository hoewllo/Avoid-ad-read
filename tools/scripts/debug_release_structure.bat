@echo off
echo ============================================
echo EPUB Ad Cleaner - 发布结构调试脚本 (Windows)
echo ============================================
echo.

REM 模拟GitHub Actions环境
echo 1. 模拟下载所有构建产物...
if exist debug_release rmdir /s /q debug_release
mkdir debug_release
cd debug_release

echo 2. 创建模拟构建产物...
mkdir windows-msvc-build
mkdir windows-mingw-build
mkdir linux-build
mkdir macos-build

echo 3. 创建模拟文件...
REM Windows MSVC
echo. > windows-msvc-build\epub_cleaner.exe
echo. > windows-msvc-build\libepub_cleaner_lib.lib
echo. > windows-msvc-build\zlib.dll

REM Windows MinGW
echo. > windows-mingw-build\epub_cleaner.exe
echo. > windows-mingw-build\libepub_cleaner_lib.a
echo. > windows-mingw-build\libz.dll

REM Linux (在Windows上模拟)
echo. > linux-build\epub_cleaner
echo. > linux-build\libepub_cleaner_lib.a
echo. > linux-build\libz.so

REM macOS (在Windows上模拟)
echo. > macos-build\epub_cleaner
echo. > macos-build\libepub_cleaner_lib.a
echo. > macos-build\libz.dylib

echo 4. 显示原始构建产物结构...
echo.
echo windows-msvc-build\:
dir /b windows-msvc-build
echo.
echo windows-mingw-build\:
dir /b windows-mingw-build
echo.
echo linux-build\:
dir /b linux-build
echo.
echo macos-build\:
dir /b macos-build

echo.
echo 5. 执行发布包创建逻辑...
echo ============================================

REM 执行与GitHub Actions相似的逻辑
mkdir release

REM 复制Windows MSVC构建 - 只复制可执行文件
if exist windows-msvc-build (
  mkdir release\windows-msvc
  copy windows-msvc-build\*.exe release\windows-msvc\ >nul 2>&1
)

REM 复制Windows MinGW构建 - 只复制可执行文件
if exist windows-mingw-build (
  mkdir release\windows-mingw
  copy windows-mingw-build\*.exe release\windows-mingw\ >nul 2>&1
)

REM 复制Linux构建 - 只复制可执行文件（在Windows上简化）
if exist linux-build (
  mkdir release\linux
  copy linux-build\epub_cleaner release\linux\ >nul 2>&1
)

REM 复制macOS构建 - 只复制可执行文件（在Windows上简化）
if exist macos-build (
  mkdir release\macos
  copy macos-build\epub_cleaner release\macos\ >nul 2>&1
)

echo.
echo 6. 显示发布目录结构...
echo ============================================
echo release\:
dir /s /b release

echo.
echo 7. 检查哪些文件会被上传到GitHub Release...
echo ============================================
echo 会被上传的文件：
if exist release\windows-msvc\epub_cleaner.exe echo release\windows-msvc\epub_cleaner.exe
if exist release\windows-mingw\epub_cleaner.exe echo release\windows-mingw\epub_cleaner.exe
if exist release\linux\epub_cleaner echo release\linux\epub_cleaner
if exist release\macos\epub_cleaner echo release\macos\epub_cleaner
echo release\README.md

echo.
echo 8. 清理...
cd ..
rmdir /s /q debug_release

echo ============================================
echo 调试完成
echo ============================================
pause