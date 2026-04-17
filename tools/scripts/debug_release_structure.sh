#!/bin/bash
echo "============================================"
echo "EPUB Ad Cleaner - 发布结构调试脚本"
echo "============================================"
echo ""

# 模拟GitHub Actions环境
echo "1. 模拟下载所有构建产物..."
mkdir -p debug_release
cd debug_release

# 创建模拟的构建产物目录结构
echo "2. 创建模拟构建产物..."
mkdir -p windows-msvc-build
mkdir -p windows-mingw-build
mkdir -p linux-build
mkdir -p macos-build

# 创建模拟文件
echo "3. 创建模拟文件..."
# Windows MSVC
touch windows-msvc-build/epub_cleaner.exe
touch windows-msvc-build/libepub_cleaner_lib.lib
touch windows-msvc-build/zlib.dll

# Windows MinGW
touch windows-mingw-build/epub_cleaner.exe
touch windows-mingw-build/libepub_cleaner_lib.a
touch windows-mingw-build/libz.dll

# Linux
touch linux-build/epub_cleaner
touch linux-build/libepub_cleaner_lib.a
touch linux-build/libz.so

# macOS
touch macos-build/epub_cleaner
touch macos-build/libepub_cleaner_lib.a
touch macos-build/libz.dylib

echo "4. 显示原始构建产物结构..."
echo ""
echo "windows-msvc-build/:"
ls -la windows-msvc-build/
echo ""
echo "windows-mingw-build/:"
ls -la windows-mingw-build/
echo ""
echo "linux-build/:"
ls -la linux-build/
echo ""
echo "macos-build/:"
ls -la macos-build/

echo ""
echo "5. 执行发布包创建逻辑..."
echo "============================================"

# 执行与GitHub Actions相同的逻辑
mkdir -p release

# 1. 为每个平台创建完整包（包含库文件和主程序）
echo "创建平台完整包..."

# Windows MSVC完整包
if [ -d "windows-msvc-build" ]; then
  mkdir -p release/windows-msvc-full
  # 复制所有文件
  find windows-msvc-build -type f -name "*" -exec cp {} release/windows-msvc-full/ \; 2>/dev/null || true
  # 创建压缩包
  cd release/windows-msvc-full && tar -czf ../epub-cleaner-windows-msvc-full.tar.gz . && cd ../..
  # 清理临时目录
  rm -rf release/windows-msvc-full
fi

# Windows MinGW完整包
if [ -d "windows-mingw-build" ]; then
  mkdir -p release/windows-mingw-full
  # 复制所有文件
  find windows-mingw-build -type f -name "*" -exec cp {} release/windows-mingw-full/ \; 2>/dev/null || true
  # 创建压缩包
  cd release/windows-mingw-full && tar -czf ../epub-cleaner-windows-mingw-full.tar.gz . && cd ../..
  # 清理临时目录
  rm -rf release/windows-mingw-full
fi

# Linux完整包
if [ -d "linux-build" ]; then
  mkdir -p release/linux-full
  # 复制所有文件
  find linux-build -type f -name "*" -exec cp {} release/linux-full/ \; 2>/dev/null || true
  # 创建压缩包
  cd release/linux-full && tar -czf ../epub-cleaner-linux-full.tar.gz . && cd ../..
  # 清理临时目录
  rm -rf release/linux-full
fi

# macOS完整包
if [ -d "macos-build" ]; then
  mkdir -p release/macos-full
  # 复制所有文件
  find macos-build -type f -name "*" -exec cp {} release/macos-full/ \; 2>/dev/null || true
  # 创建压缩包
  cd release/macos-full && tar -czf ../epub-cleaner-macos-full.tar.gz . && cd ../..
  # 清理临时目录
  rm -rf release/macos-full
fi

# 2. 为每个平台创建主程序单独包
echo "创建主程序单独包..."

# Windows MSVC主程序包
if [ -d "windows-msvc-build" ]; then
  mkdir -p release/windows-msvc-app
  # 只复制可执行文件
  find windows-msvc-build -name "*.exe" -type f -exec cp {} release/windows-msvc-app/ \; 2>/dev/null || true
  # 创建压缩包
  cd release/windows-msvc-app && tar -czf ../epub-cleaner-windows-msvc-app.tar.gz . && cd ../..
  # 清理临时目录
  rm -rf release/windows-msvc-app
fi

# Windows MinGW主程序包
if [ -d "windows-mingw-build" ]; then
  mkdir -p release/windows-mingw-app
  # 只复制可执行文件
  find windows-mingw-build -name "*.exe" -type f -exec cp {} release/windows-mingw-app/ \; 2>/dev/null || true
  # 创建压缩包
  cd release/windows-mingw-app && tar -czf ../epub-cleaner-windows-mingw-app.tar.gz . && cd ../..
  # 清理临时目录
  rm -rf release/windows-mingw-app
fi

# Linux主程序包
if [ -d "linux-build" ]; then
  mkdir -p release/linux-app
  # 只复制可执行文件（无扩展名）
  find linux-build -type f -executable ! -name "*.so" ! -name "*.a" ! -name "*.o" -exec cp {} release/linux-app/ \; 2>/dev/null || true
  # 创建压缩包
  cd release/linux-app && tar -czf ../epub-cleaner-linux-app.tar.gz . && cd ../..
  # 清理临时目录
  rm -rf release/linux-app
fi

# macOS主程序包
if [ -d "macos-build" ]; then
  mkdir -p release/macos-app
  # 只复制可执行文件（无扩展名）
  find macos-build -type f -executable ! -name "*.dylib" ! -name "*.a" ! -name "*.o" -exec cp {} release/macos-app/ \; 2>/dev/null || true
  # 创建压缩包
  cd release/macos-app && tar -czf ../epub-cleaner-macos-app.tar.gz . && cd ../..
  # 清理临时目录
  rm -rf release/macos-app
fi

echo ""
echo "6. 显示发布目录结构..."
echo "============================================"
echo "release/:"
find release -type f | sort

echo ""
echo "7. 检查哪些文件会被上传到GitHub Release..."
echo "============================================"
echo "会被上传的文件："
# 列出所有压缩包
for file in release/*.tar.gz; do
  if [ -f "$file" ]; then
    echo "$file"
  fi
done
echo "release/README.md"

echo ""
echo "8. 清理..."
cd ..
rm -rf debug_release

echo "============================================"
echo "调试完成"
echo "============================================"