# EPUB广告清理工具 - 重构版本使用说明

## 项目重构概述

本次重构对项目进行了以下改进：

### 1. 代码结构优化
- 移除了重复的 `file_utils_old.cpp` 文件
- 将ZIP操作分离到独立的 `zip_utils` 模块
- 添加了日志系统 (`logger.h/cpp`)
- 改进了错误处理和异常安全

### 2. 功能增强
- 支持更多命令行选项
- 改进的日志输出（支持不同级别）
- 更好的错误消息和调试信息
- 添加了单元测试框架

### 3. 构建系统改进
- 更新了CMake配置
- 支持测试构建选项
- 改进的Windows构建脚本

## 快速开始

### 编译项目

```bash
# 使用构建脚本（Windows）
build.bat

# 或手动使用CMake
mkdir build
cd build
cmake ..
cmake --build .
```

### 基本使用

```bash
# 清理单个EPUB文件
epub_cleaner -i input.epub -o output.epub

# 批量处理目录
epub_cleaner -I ./books -O ./cleaned_books

# 使用详细输出
epub_cleaner -i input.epub -o output.epub -v

# 使用自定义广告模式
epub_cleaner -i input.epub -p my_patterns.txt
```

## 新功能

### 1. 日志级别控制

```bash
# 静默模式（只显示错误）
epub_cleaner -q -i input.epub -o output.epub

# 详细模式
epub_cleaner -v -i input.epub -o output.epub

# 调试模式
epub_cleaner -d -i input.epub -o output.epub
```

### 2. 列出内置广告模式

```bash
epub_cleaner --list-patterns
```

### 3. 测试功能

```bash
# 运行单元测试
test_epub_cleaner
```

## 命令行选项

### 输入/输出选项
- `-i, --input FILE` - 输入EPUB文件路径
- `-o, --output FILE` - 输出EPUB文件路径
- `-I, --input-dir DIR` - 输入目录（批量处理）
- `-O, --output-dir DIR` - 输出目录（批量处理）

### 广告模式选项
- `-p, --patterns FILE` - 自定义广告模式文件
- `--list-patterns` - 列出所有内置广告模式

### 日志和输出选项
- `-v, --verbose` - 启用详细输出
- `-q, --quiet` - 静默模式，只显示错误
- `-d, --debug` - 启用调试输出

### 其他选项
- `-n, --no-backup` - 不创建备份文件
- `-h, --help` - 显示帮助信息
- `-V, --version` - 显示版本信息

## 项目结构

```
epub_cleaner/
├── docs/                    # 文档
├── include/                # 头文件
│   ├── epub_processor.h   # EPUB处理器
│   ├── ad_patterns.h      # 广告模式管理
│   ├── file_utils.h       # 文件操作工具
│   ├── zip_utils.h        # ZIP操作工具
│   └── logger.h           # 日志系统
├── src/                   # 源代码
│   ├── main.cpp          # 主程序入口
│   ├── epub_processor.cpp
│   ├── ad_patterns.cpp
│   ├── file_utils.cpp
│   ├── zip_utils.cpp
│   └── logger.cpp
├── tests/                 # 单元测试
│   └── test_main.cpp
├── CMakeLists.txt        # CMake构建配置
├── build.bat             # Windows构建脚本
├── compile_simple.bat    # 简单编译脚本
└── REFACTORED_USAGE.md   # 本文档
```

## 广告模式文件格式

自定义广告模式文件是纯文本文件，每行一个正则表达式模式：

```
# 注释以#开头
# 每行一个正则表达式模式

【.*广告.*】
https?://.*\.com.*
\[推广\].*\[/推广\]
```

## 错误处理

重构后的程序提供了更好的错误处理：

1. **详细的错误消息** - 包含上下文信息
2. **异常安全** - 使用RAII管理资源
3. **日志分级** - 不同级别的错误信息
4. **状态统计** - 处理过程中的统计信息

## 测试

### 运行单元测试

```bash
# 编译时启用测试
build.bat  # 选择'y'构建测试

# 运行测试
test_epub_cleaner
```

### 集成测试

```bash
# 使用Python集成测试
python test_integration.py
```

## 故障排除

### 常见问题

1. **无法找到编译器**
   - 确保安装了Visual Studio或MinGW
   - 检查PATH环境变量

2. **CMake配置失败**
   - 更新CMake到最新版本
   - 检查CMakeLists.txt语法

3. **ZIP操作失败**
   - 确保系统安装了PowerShell（Windows）或unzip/zip命令（Linux）
   - 检查文件权限

4. **正则表达式错误**
   - 检查自定义模式文件格式
   - 使用简单的模式测试

## 贡献指南

1. 遵循现有的代码风格
2. 添加新功能时包含单元测试
3. 更新相关文档
4. 使用有意义的提交信息

## 许可证

MIT License - 详见 `docs/LICENSE`

---

**重构版本**: v2.0.0
**重构日期**: 2024年1月
**状态**: ✅ 重构完成