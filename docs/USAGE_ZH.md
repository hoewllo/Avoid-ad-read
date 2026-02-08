# EPUB广告清理工具 - 使用指南 (v1.1.0)

## 快速开始

### 1. 编译程序

#### Windows
```bash
# 方法1: 使用简单编译脚本
tools/build-tool/compile_simple.bat

# 方法2: 使用完整构建脚本
tools/build-tool/build.bat

# 方法3: 手动CMake
mkdir build
cd build
cmake .. -G \"Visual Studio 17 2022\" -A x64
cmake --build . --config Release
```

#### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make
```

### 2. 基本使用

```bash
# 清理单个EPUB文件
epub_cleaner -i input.epub -o output.epub

# 批量处理目录
epub_cleaner -I ./input_dir -O ./output_dir

# 启用详细输出
epub_cleaner -i input.epub -v

# 列出内置广告模式
epub_cleaner --list-patterns

# 启用调试模式
epub_cleaner -i input.epub -d

# 静默模式（仅显示错误）
epub_cleaner -i input.epub -q
```

## 详细功能说明

### 广告模式

工具内置7种广告模式，可以识别以下类型的广告内容：

1. **GitHub项目推广**: 包含GitHub链接和项目名称
2. **下载广告**: 包含关键字\"download\"
3. **特殊字符**: 零宽字符和其他特殊Unicode字符
4. **空括号**: 可能的隐藏广告标记

### 自定义广告模式

您可以创建自定义广告模式文件：

```bash
# 创建模式文件 custom_patterns.txt
# 每行一个正则表达式

# 示例：匹配特定广告
【.*ad.*】
【.*promotion.*】

# 使用自定义模式
epub_cleaner -i input.epub -p custom_patterns.txt
```

### 备份机制

默认情况下，程序会为每个处理的文件创建.bak备份文件。

```bash
# 禁用备份
epub_cleaner -i input.epub -n

# 备份文件位置：与原始文件相同目录
input.epub      # 原始文件
input.epub.bak  # 备份文件
```

## v1.1.0 新功能

### 增强的ZIP处理
- **新的zip_utils模块**: 改进的ZIP文件处理，支持平台特定实现
- **更好的错误处理**: ZIP操作提供更详细的错误信息
- **后备机制**: 多种ZIP解压/压缩后备方法

### 高级日志系统
- **多级日志**: DEBUG、INFO、WARN、ERROR、FATAL级别
- **可配置输出**: 使用-v、-d、-q标志控制日志详细程度
- **时间戳支持**: 日志消息中可选时间戳

### 改进的文件工具
- **增强的文件操作**: 更好的文件和目录管理
- **跨平台兼容性**: 改进对Windows和Unix系统的支持
- **错误恢复**: 更好的文件系统错误处理

## 高级用法

### 批量处理脚本

创建批处理脚本 `clean_all.bat` (Windows)：
```batch
@echo off
setlocal

set INPUT_DIR=books
set OUTPUT_DIR=cleaned_books
set LOG_FILE=cleanup.log

echo 开始批量EPUB清理...
echo 输入目录: %INPUT_DIR%
echo 输出目录: %OUTPUT_DIR%
echo.

if not exist \"%OUTPUT_DIR%\" mkdir \"%OUTPUT_DIR%\"

epub_cleaner -I \"%INPUT_DIR%\" -O \"%OUTPUT_DIR%\" -v > \"%LOG_FILE%\" 2>&1

if errorlevel 1 (
    echo 处理失败，请检查日志文件: %LOG_FILE%
) else (
    echo 处理完成！日志文件: %LOG_FILE%
)

pause
```

### 集成到工作流

您可以将此工具集成到电子书处理管道中：

```bash
#!/bin/bash
# ebook_pipeline.sh

INPUT=\"$1\"
OUTPUT=\"${INPUT%.epub}_clean.epub\"
LOG=\"clean_$(date +%Y%m%d_%H%M%S).log\"

# 步骤1: 清理广告
epub_cleaner -i \"$INPUT\" -o \"$OUTPUT\" -v > \"$LOG\" 2>&1

if [ $? -eq 0 ]; then
    echo \"广告清理完成: $OUTPUT\"
    
    # 步骤2: 可选的其他处理
    # ...
    
    # 步骤3: 验证结果
    echo \"验证清理结果...\"
    grep -q \"【\" \"$OUTPUT\" && echo \"警告: 可能有广告残留\" || echo \"验证通过\"
else
    echo \"清理失败，请检查日志: $LOG\"
    exit 1
fi
```

## 性能优化

### 处理大文件

对于包含许多XHTML文件的大型EPUB：

```bash
# 使用较少内存（逐文件处理）
epub_cleaner -i large_book.epub --no-backup

# 批量处理时限制并发（如果需要）
# 注意：当前版本是单线程的
```

### 磁盘空间

- 临时目录：程序创建临时目录，需要足够的磁盘空间
- 备份文件：每个文件创建备份，确保有足够空间

## 故障排除

### 常见错误

1. **\"无法打开文件\"**
   - 检查文件路径是否正确
   - 检查文件权限
   
2. **\"正则表达式错误\"**
   - 检查自定义模式文件中的正则表达式语法
   - 尝试简化模式
   
3. **\"内存不足\"**
   - 可能在处理非常大的文件时发生
   - 尝试分批处理

4. **\"ZIP解压失败\"**
   - 确保系统已安装ZIP工具
   - 检查文件权限
   - 尝试以管理员权限运行

### 调试技巧

1. **启用详细输出**: 使用 `-v` 参数
2. **启用调试模式**: 使用 `-d` 参数获取详细日志
3. **检查临时文件**: 程序在系统临时目录中创建文件
4. **查看日志**: 将输出重定向到日志文件

## 示例

### 示例1: 基本清理

```bash
# 输入文件: book.epub
# 输出文件: book_clean.epub
# 创建备份: 是
# 详细输出: 否

epub_cleaner -i book.epub
```

### 示例2: 带日志的批量处理

```bash
# 处理目录中的所有EPUB文件
# 输出到新目录
# 记录详细日志

epub_cleaner -I ./my_books -O ./cleaned_books -v > cleanup.log
```

### 示例3: 使用自定义模式清理并调试

```bash
# 使用自定义广告模式
# 不创建备份
# 启用调试输出

epub_cleaner -i book.epub -o clean.epub -p my_patterns.txt -n -d
```

### 示例4: 静默模式处理

```bash
# 静默模式处理（仅显示错误）
# 适用于自动化脚本

epub_cleaner -i book.epub -o clean.epub -q
```

## 更新和维护

### 更新广告模式

1. 编辑 `example_patterns.txt` 文件
2. 添加新的正则表达式模式
3. 使用 `-p` 参数指定模式文件

### 报告问题

如果发现广告清理不完整或程序问题：

1. 保存原始EPUB文件
2. 记录使用的命令和参数
3. 提供错误消息或日志
4. 报告具体的广告内容模式

## 测试

### 单元测试
```bash
# 编译并运行单元测试
cd tools/test
g++ -std=c++17 -I../../include -I../.. test_main.cpp ../../src/file_utils.cpp ../../src/ad_patterns.cpp ../../src/logger.cpp -o test_runner
test_runner.exe
```

### 集成测试
```bash
# 运行重构版本测试
tools/test/test_refactored.bat
```

## 许可证

本项目使用MIT许可证。详见LICENSE文件。

## 贡献

欢迎贡献代码、报告问题或提出改进建议！

---

**项目状态**: ✅ 已完成并重构  
**版本**: v1.1.0  
**最后更新**: 2024年1月26日  
**构建工具位置**: tools/build-tool/  
**测试工具位置**: tools/test/