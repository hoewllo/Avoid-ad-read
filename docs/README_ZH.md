# EPUB广告清理工具 (C++版本)

一个用C++编写的EPUB广告内容清理工具，支持批量处理、自动解压、广告清理和重新打包。

## 功能特性

- **自动解压EPUB文件**：将EPUB文件解压到临时目录
- **智能广告检测**：使用正则表达式匹配多种广告模式
- **批量处理**：支持单个文件或目录批量处理
- **备份机制**：自动创建.bak备份文件
- **重新打包**：清理后重新打包为干净的EPUB文件
- **参数化配置**：支持命令行参数灵活配置

## 广告模式识别

工具可以识别并清理以下类型的广告内容：

1. **GitHub项目推广广告**：
   - 格式：`【使用xxx项目进行下载：https://github.com/xxx】`
   - 示例：`【使用POf-L/Fanqie-novel-Downloader项目进行下载：https://github.com/POf-L/Fanqie-novel-Downloader】`

2. **特殊Unicode字符**：
   - 零宽字符：`\u200b`, `\u200c`, `\u200d`, `\u2060`, `\ufeff`
   - 其他特殊字符：`‌`, `᠎`, `⁠`, `﻿`

3. **通用广告模式**：
   - 包含"下载"、"项目"、"使用"等关键词的方括号内容

## 安装与编译

### 依赖项
- C++17 或更高版本
- zlib (用于EPUB压缩/解压)
- libzip (可选，用于ZIP操作)
- 支持正则表达式的标准库

### 编译命令
```bash
# 使用CMake
mkdir build
cd build
cmake ..
cmake --build .

# 或直接使用g++
g++ -std=c++17 -o epub_cleaner main.cpp -lz
```

## 使用方法

### 基本用法
```bash
# 清理单个EPUB文件
epub_cleaner --input book.epub --output clean_book.epub

# 批量处理目录中的所有EPUB文件
epub_cleaner --input-dir ./books --output-dir ./cleaned_books

# 启用详细输出
epub_cleaner --input book.epub --output clean_book.epub --verbose

# 不创建备份文件
epub_cleaner --input book.epub --output clean_book.epub --no-backup
```

### 命令行参数

| 参数 | 缩写 | 描述 | 默认值 |
|------|------|------|--------|
| `--input` | `-i` | 输入EPUB文件路径 | 无 |
| `--output` | `-o` | 输出EPUB文件路径 | 输入文件加"_clean"后缀 |
| `--input-dir` | `-I` | 输入目录（批量处理） | 无 |
| `--output-dir` | `-O` | 输出目录（批量处理） | 输入目录加"_cleaned"后缀 |
| `--verbose` | `-v` | 启用详细输出 | false |
| `--no-backup` | `-n` | 不创建备份文件 | false |
| `--patterns` | `-p` | 自定义广告模式文件 | 使用内置模式 |
| `--help` | `-h` | 显示帮助信息 | 无 |

## 项目结构

```
epub_cleaner/
├── src/
│   ├── main.cpp              # 主程序入口
│   ├── epub_processor.cpp    # EPUB处理核心逻辑
│   ├── epub_processor.h
│   ├── ad_patterns.cpp       # 广告模式定义
│   ├── ad_patterns.h
│   ├── file_utils.cpp        # 文件工具函数
│   └── file_utils.h
├── include/                  # 头文件
├── tests/                   # 测试文件
├── CMakeLists.txt           # CMake构建配置
└── README.md               # 项目说明
```

## 技术实现

### 1. EPUB文件结构处理
- 使用zlib进行ZIP压缩/解压
- 解析EPUB的mimetype和container.xml
- 遍历OPF文件获取所有XHTML内容文件

### 2. 广告清理算法
- 基于正则表达式的多模式匹配
- 支持Unicode字符处理
- 保留原始文件格式和编码

### 3. 错误处理
- 全面的错误检查和异常处理
- 详细的错误信息输出
- 清理临时文件防止资源泄漏

## 示例

### 清理前 (chapter_39.xhtml片段)
```html
<p>小说正文内容...</p>
<p>【使用POf-L/Fanqie-novel-Downloader项目进行下载：https://github.com/POf-L/Fanqie-novel-Downloader】</p>
<p>继续小说内容...</p>
```

### 清理后
```html
<p>小说正文内容...</p>
<p>继续小说内容...</p>
```

## 性能优化

- 使用内存映射文件提高大文件处理速度
- 多线程支持批量处理
- 增量更新避免全文件重写

## 许可证

MIT License

## 贡献指南

1. Fork 项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 问题反馈

如遇到问题或有改进建议，请在GitHub Issues中提交。
```

## 更新日志

### v1.0.0 (2024-01-26)
- 初始版本发布
- 支持基本EPUB广告清理功能
- 实现命令行参数处理
- 添加备份机制