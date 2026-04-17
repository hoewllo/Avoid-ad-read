#!/bin/bash
echo "============================================"
echo "测试新的打包策略"
echo "============================================"
echo ""

# 创建测试目录结构
mkdir -p test_packaging
cd test_packaging

# 创建模拟文件
echo "1. 创建模拟构建文件..."
mkdir -p windows-msvc-build
mkdir -p windows-mingw-build
mkdir -p linux-build
mkdir -p macos-build

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

echo "2. 测试新的打包逻辑..."
echo ""

# 测试Windows MSVC打包
echo "测试Windows MSVC打包:"
if [ -d "windows-msvc-build" ]; then
  # 完整包
  mkdir -p release/windows-msvc-full
  find windows-msvc-build -type f -name "*" -exec cp {} release/windows-msvc-full/ \; 2>/dev/null || true
  cd release/windows-msvc-full && tar -czf ../epub-cleaner-windows-msvc-full.tar.gz . && cd ../..
  rm -rf release/windows-msvc-full
  
  # 主程序包
  mkdir -p release/windows-msvc-app
  find windows-msvc-build -name "*.exe" -type f -exec cp {} release/windows-msvc-app/ \; 2>/dev/null || true
  cd release/windows-msvc-app && tar -czf ../epub-cleaner-windows-msvc-app.tar.gz . && cd ../..
  rm -rf release/windows-msvc-app
  
  echo "  ✓ 创建了Windows MSVC包"
fi

# 测试Linux打包
echo "测试Linux打包:"
if [ -d "linux-build" ]; then
  # 完整包
  mkdir -p release/linux-full
  find linux-build -type f -name "*" -exec cp {} release/linux-full/ \; 2>/dev/null || true
  cd release/linux-full && tar -czf ../epub-cleaner-linux-full.tar.gz . && cd ../..
  rm -rf release/linux-full
  
  # 主程序包
  mkdir -p release/linux-app
  find linux-build -type f -executable ! -name "*.so" ! -name "*.a" ! -name "*.o" -exec cp {} release/linux-app/ \; 2>/dev/null || true
  cd release/linux-app && tar -czf ../epub-cleaner-linux-app.tar.gz . && cd ../..
  rm -rf release/linux-app
  
  echo "  ✓ 创建了Linux包"
fi

echo ""
echo "3. 检查生成的包..."
echo "============================================"
echo "生成的包:"
find release -name "*.tar.gz" -type f | sort

echo ""
echo "4. 检查包内容..."
echo "============================================"

# 检查Windows MSVC完整包
if [ -f "release/epub-cleaner-windows-msvc-full.tar.gz" ]; then
  echo "Windows MSVC完整包内容:"
  tar -tzf release/epub-cleaner-windows-msvc-full.tar.gz
  echo ""
fi

# 检查Windows MSVC主程序包
if [ -f "release/epub-cleaner-windows-msvc-app.tar.gz" ]; then
  echo "Windows MSVC主程序包内容:"
  tar -tzf release/epub-cleaner-windows-msvc-app.tar.gz
  echo ""
fi

# 检查Linux完整包
if [ -f "release/epub-cleaner-linux-full.tar.gz" ]; then
  echo "Linux完整包内容:"
  tar -tzf release/epub-cleaner-linux-full.tar.gz
  echo ""
fi

# 检查Linux主程序包
if [ -f "release/epub-cleaner-linux-app.tar.gz" ]; then
  echo "Linux主程序包内容:"
  tar -tzf release/epub-cleaner-linux-app.tar.gz
  echo ""
fi

echo "5. 清理..."
cd ..
rm -rf test_packaging

echo "============================================"
echo "测试完成"
echo "============================================"