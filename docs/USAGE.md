# EPUB广告清理工具 - 使用指南

## 快速开始

### 1. 编译程序

#### Windows
```bash
# 方法1: 使用构建脚本
build.bat

# 方法2: 手动使用CMake
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
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
```

## 详细功能说明

### 广告模式

工具内置了7种广告模式，可以识别以下类型的广告内容：

1. **GitHub项目推广**：包含GitHub链接和项目名称
2. **下载广告**：包含"下载"关键词
3. **特殊字符**：零宽字符等特殊Unicode字符
4. **空方括号**：可能被隐藏的广告标记

### 自定义广告模式

您可以创建自定义广告模式文件：

```bash
# 创建模式文件 custom_patterns.txt
# 每行一个正则表达式

# 示例：匹配特定广告
【.*广告.*】
【.*推广.*】

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

## 高级用法

### 批量处理脚本

创建批处理脚本 `clean_all.bat` (Windows)：
```batch
@echo off
setlocal

set INPUT_DIR=books
set OUTPUT_DIR=cleaned_books
set LOG_FILE=cleanup.log

echo 开始批量清理EPUB文件...
echo 输入目录: %INPUT_DIR%
echo 输出目录: %OUTPUT_DIR%
echo.

if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

epub_cleaner -I "%INPUT_DIR%" -O "%OUTPUT_DIR%" -v > "%LOG_FILE%" 2>&1

if errorlevel 1 (
    echo 处理失败，请查看日志文件: %LOG_FILE%
) else (
    echo 处理完成! 日志文件: %LOG_FILE%
)

pause
```

### 集成到工作流

您可以将此工具集成到电子书处理流水线中：

```bash
#!/bin/bash
# ebook_pipeline.sh

INPUT="$1"
OUTPUT="${INPUT%.epub}_clean.epub"
LOG="clean_$(date +%Y%m%d_%H%M%S).log"

# 步骤1: 清理广告
epub_cleaner -i "$INPUT" -o "$OUTPUT" -v > "$LOG" 2>&1

if [ $? -eq 0 ]; then
    echo "广告清理完成: $OUTPUT"
    
    # 步骤2: 可选的其他处理
    # ...
    
    # 步骤3: 验证结果
    echo "验证清理结果..."
    grep -q "【" "$OUTPUT" && echo "警告: 可能还有广告残留" || echo "验证通过"
else
    echo "清理失败，请查看日志: $LOG"
    exit 1
fi
```

## 性能优化

### 处理大量文件

对于包含大量XHTML文件的大型EPUB：

```bash
# 使用更少的内存（逐文件处理）
epub_cleaner -i large_book.epub --no-backup

# 批量处理时限制并发（如果需要）
# 注意：当前版本是单线程的
```

### 磁盘空间

- 临时目录：程序会创建临时目录，需要足够的磁盘空间
- 备份文件：每个文件都会创建备份，确保有足够空间

## 故障排除

### 常见错误

1. **"无法打开文件"**
   - 检查文件路径是否正确
   - 检查文件权限
   
2. **"正则表达式错误"**
   - 检查自定义模式文件中的正则表达式语法
   - 尝试简化模式
   
3. **"内存不足"**
   - 处理非常大的文件时可能出现
   - 尝试分批次处理

### 调试技巧

1. **启用详细输出**：使用 `-v` 参数
2. **检查临时文件**：程序会在系统临时目录创建文件
3. **查看日志**：重定向输出到日志文件

## 示例

### 示例1：基本清理

```bash
# 输入文件: book.epub
# 输出文件: book_clean.epub
# 创建备份: 是
# 详细输出: 否

e pub_cleaner -i book.epub
```

### 示例2：批量处理并记录

```bash
# 处理目录中的所有EPUB文件
# 输出到新目录
# 记录详细日志

e pub_cleaner -I ./my_books -O ./cleaned_books -v > cleanup.log
```

### 示例3：自定义模式清理

```bash
# 使用自定义广告模式
# 不创建备份
# 输出到指定文件

e pub_cleaner -i book.epub -o clean.epub -p my_patterns.txt -n
```

## 更新与维护

### 更新广告模式

1. 编辑 `example_patterns.txt` 文件
2. 添加新的正则表达式模式
3. 使用 `-p` 参数指定模式文件

### 报告问题

如果您发现广告清理不彻底或程序有问题：

1. 保存原始EPUB文件
2. 记录使用的命令和参数
3. 提供错误信息或日志
4. 报告具体的广告内容模式

## 许可证

本项目使用MIT许可证。详情请查看LICENSE文件。

## 贡献

欢迎贡献代码、报告问题或提出改进建议！