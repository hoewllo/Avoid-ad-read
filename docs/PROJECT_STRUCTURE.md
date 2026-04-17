# EPUB Ad Cleaner 项目结构

## 目录结构

```
epub-ad-cleaner/
├── CMakeLists.txt          # CMake主配置文件
├── CMakePresets.json       # CMake预设配置
├── project.config.json     # 项目配置文件
├── VERSION                 # 版本文件
├── LICENSE                 # 许可证文件
├── README.md               # 项目说明文档
├── PROJECT_STRUCTURE.md    # 项目结构文档（本文档）
├── .gitignore             # Git忽略文件
├── build.bat              # Windows构建入口脚本
├── scripts/               # 脚本目录
│   ├── build/            # 构建脚本
│   │   └── build.ps1     # PowerShell构建脚本
│   └── read_version.bat  # 版本读取脚本
├── cmake/                 # CMake模块和工具链
│   ├── cmake_uninstall.cmake.in
│   ├── epub_cleaner-config.cmake.in
│   ├── toolchain-example.cmake
│   ├── toolchain-mingw32.cmake
│   ├── toolchain-mingw64.cmake
│   └── version.rc.in
├── include/               # 头文件目录
│   ├── ad_patterns.h     # 广告模式处理
│   ├── epub_processor.h  # EPUB处理器
│   ├── file_utils.h      # 文件工具
│   ├── iconv_wrapper.h   # 编码转换包装器
│   ├── logger.h          # 日志系统
│   ├── version.h         # 版本信息
│   └── zip_utils.h       # ZIP工具
├── src/                  # 源代码目录
│   ├── ad_patterns.cpp
│   ├── epub_processor.cpp
│   ├── file_utils.cpp
│   ├── iconv_wrapper.cpp
│   ├── logger.cpp
│   ├── main.cpp          # 程序入口点
│   ├── zip_utils_impl.cpp
│   └── zlib_utils.cpp
├── test/                 # 测试代码
│   └── test_basic.cpp
├── tools/                # 工具和示例
│   ├── build-tool/      # 旧版构建工具
│   │   ├── build.bat
│   │   └── compile_simple.bat
│   ├── test/            # 测试工具
│   │   ├── test_main.cpp
│   │   └── test_refactored.bat
│   └── example_patterns.txt  # 示例广告模式
└── docs/                 # 文档目录
    └── BUILD.md         # 构建指南
```

## 构建系统

### 主要构建方式

1. **统一构建脚本** (`build.bat` / `scripts/build/build.ps1`)
   - 支持所有CMake预设
   - 提供清理、构建、测试、安装功能
   - PowerShell脚本，支持跨平台

2. **CMake预设** (`CMakePresets.json`)
   - `default`: 默认配置（MinGW + Release）
   - `debug`: 调试配置（包含符号和测试）
   - `release`: 发布配置（优化）
   - `msvc`: Visual Studio配置
   - `minimal`: 最小配置（无可选依赖）

3. **传统构建脚本** (`tools/build-tool/`)
   - 旧版构建脚本，保持向后兼容

### 构建命令示例

```bash
# 使用统一构建脚本
.\build.bat
.\build.bat -Preset debug -Test
.\build.bat -Clean

# 使用CMake命令
cmake --preset default
cmake --build --preset default
ctest --preset default

# 使用PowerShell脚本
powershell -ExecutionPolicy Bypass -File scripts\build\build.ps1 -Preset release -Install
```

## 源代码组织

### 核心模块

1. **epub_processor** - EPUB文件处理核心
   - 解压、清理、重新打包EPUB文件
   - 支持批量处理
   - 统计信息收集

2. **ad_patterns** - 广告模式匹配
   - 正则表达式模式匹配
   - 支持自定义模式文件
   - 内置常见广告模式

3. **file_utils** - 文件操作工具
   - 文件系统操作
   - 路径处理
   - 备份管理

4. **iconv_wrapper** - 编码转换
   - 字符编码转换
   - 支持多种编码格式
   - 可选的依赖项

5. **zip_utils** - ZIP压缩处理
   - EPUB文件解压/压缩
   - 支持系统ZIP工具或内置实现

6. **logger** - 日志系统
   - 分级日志输出
   - 控制台和文件日志
   - 可配置的详细级别

### 依赖管理

- **必需依赖**: C++17标准库
- **可选依赖**: 
  - ZLIB (压缩支持，通过 `ENABLE_ZLIB` 控制)
  - Iconv (编码转换，通过 `ENABLE_ICONV` 控制)
  - 系统ZIP工具 (通过 `ENABLE_SYSTEM_ZIP` 控制)

## 开发工作流

### 1. 环境设置
```bash
# 克隆项目
git clone <repository-url>
cd epub-ad-cleaner

# 安装构建工具
# - CMake 3.15+
# - C++编译器 (GCC, Clang, MSVC)
# - 可选: ZLIB, Iconv开发库
```

### 2. 构建和测试
```bash
# 首次构建
.\build.bat

# 调试构建
.\build.bat -Preset debug -Test

# 清理并重新构建
.\build.bat -Clean
.\build.bat -Preset release
```

### 3. 代码结构约定
- 头文件放在 `include/` 目录
- 源文件放在 `src/` 目录
- 测试代码放在 `test/` 目录
- 工具脚本放在 `scripts/` 目录
- 使用命名空间 `epub_cleaner`

## 文件说明

### 关键文件

1. **CMakeLists.txt** - 主构建配置
   - 项目设置和版本管理
   - 编译器选项和警告设置
   - 依赖查找和链接
   - 目标定义和安装规则

2. **CMakePresets.json** - 预设配置
   - 预定义的构建配置
   - 跨平台支持
   - 简化的构建命令

3. **project.config.json** - 项目元数据
   - 项目信息和版本
   - 功能特性描述
   - 依赖关系说明

4. **VERSION** - 版本控制
   - 简单版本号文件
   - 格式: `MAJOR.MINOR.PATCH`
   - 被CMake和代码读取

## 扩展和定制

### 添加新功能
1. 在 `include/` 中添加头文件
2. 在 `src/` 中添加实现文件
3. 更新 `CMakeLists.txt` 中的源文件列表
4. 添加相应的测试

### 自定义构建
1. 修改 `CMakePresets.json` 添加新预设
2. 更新 `CMakeLists.txt` 中的选项
3. 扩展 `scripts/build/build.ps1` 脚本

### 添加依赖
1. 在 `CMakeLists.txt` 中添加 `find_package()`
2. 添加相应的CMake选项
3. 更新 `project.config.json` 中的依赖列表