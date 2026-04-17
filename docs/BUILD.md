# EPUB Cleaner 构建指南

## 快速开始

### 使用 CMake Presets（推荐）

```bash
# 配置项目（使用默认预设）
cmake --preset default

# 构建项目
cmake --build --preset default

# 安装
cmake --install build/default

# 运行测试（如果启用）
ctest --preset default
```

### 使用传统 CMake 命令

```bash
# 创建构建目录并配置
cmake -B build -S .

# 构建项目
cmake --build build

# 安装
cmake --install build

# 运行测试
cd build && ctest
```

## 可用预设

项目提供了多个预设配置：

### 配置预设
- `default` - 默认配置（Ninja + Release）
- `debug` - 调试配置（包含符号和测试）
- `release` - 发布配置（优化）
- `msvc` - Visual Studio 配置
- `minimal` - 最小配置（无可选依赖）

### 构建预设
- `default` - 构建默认配置
- `debug` - 构建调试版本
- `release` - 构建发布版本
- `msvc` - 使用 MSVC 构建
- `minimal` - 构建最小版本

## 构建选项

可以通过 CMake 变量自定义构建：

```bash
# 启用测试
cmake -B build -S . -DBUILD_TESTS=ON

# 禁用 ZLIB 支持
cmake -B build -S . -DENABLE_ZLIB=OFF

# 指定安装路径
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr/local

# 构建共享库
cmake -B build -S . -DBUILD_SHARED_LIBS=ON
```

### 主要选项
- `BUILD_SHARED_LIBS` - 构建共享库（默认：OFF）
- `BUILD_TESTS` - 构建测试（默认：OFF）
- `BUILD_DOCS` - 构建文档（默认：OFF）
- `ENABLE_ZLIB` - 启用 ZLIB 压缩支持（默认：ON）
- `ENABLE_ICONV` - 启用 Iconv 编码转换（默认：ON）
- `ENABLE_SYSTEM_ZIP` - 使用系统 ZIP 工具（默认：ON）

## 依赖管理

### 必需依赖
- C++17 兼容编译器
- CMake 3.15+

### 可选依赖
- **ZLIB** - 压缩支持（通过 `ENABLE_ZLIB` 控制）
- **Iconv** - 字符编码转换（通过 `ENABLE_ICONV` 控制）

## 跨平台构建

### Windows
```bash
# 使用 Visual Studio
cmake --preset msvc
cmake --build --preset msvc

# 或使用 Ninja
cmake --preset default
cmake --build --preset default
```

### Linux/macOS
```bash
# 安装 Ninja（如果未安装）
# Ubuntu/Debian: sudo apt install ninja-build
# macOS: brew install ninja

cmake --preset default
cmake --build --preset default
```

## 安装和打包

### 安装到系统
```bash
# 安装到默认位置
cmake --install build

# 安装到指定目录
cmake --install build --prefix /opt/epub_cleaner
```

### 创建包
```bash
# 创建压缩包
cd build && cpack -G TGZ

# 创建多种格式
cd build && cpack -G TGZ -G ZIP
```

## 开发工作流

### 1. 开发构建
```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset default
```

### 2. 发布构建
```bash
cmake --preset release
cmake --build --preset release
cmake --install build/release
```

### 3. 最小构建（无依赖）
```bash
cmake --preset minimal
cmake --build --preset minimal
```

## 故障排除

### 常见问题

1. **找不到 ZLIB**
   ```bash
   # Ubuntu/Debian
   sudo apt install zlib1g-dev
   
   # macOS
   brew install zlib
   
   # 或禁用 ZLIB
   cmake -B build -S . -DENABLE_ZLIB=OFF
   ```

2. **找不到 Iconv**
   ```bash
   # Ubuntu/Debian
   sudo apt install libiconv-dev
   
   # 或使用内置编码转换
   cmake -B build -S . -DENABLE_ICONV=OFF
   ```

3. **构建速度慢**
   ```bash
   # 使用 Ninja 替代 Make
   cmake -B build -S . -G Ninja
   
   # 使用多核编译
   cmake --build build --parallel 8
   ```

### 清理构建
```bash
# 删除构建目录
rm -rf build/

# 或使用 CMake
cmake --build build --target clean
```

## 高级用法

### 自定义编译器
```bash
# 指定编译器
CC=clang CXX=clang++ cmake -B build -S .
```

### 交叉编译
```bash
# 设置工具链文件
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake
```

### 生成编译数据库
```bash
# 用于 clangd 等工具
cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

## 环境变量

- `CMAKE_PREFIX_PATH` - 额外搜索依赖的路径
- `CMAKE_INSTALL_PREFIX` - 安装路径
- `CC` / `CXX` - 指定 C/C++ 编译器

## 相关文件

- `CMakeLists.txt` - 主构建配置
- `CMakePresets.json` - 预设配置
- `cmake/` - CMake 模块和模板
- `tools/build-tool/` - 构建工具脚本