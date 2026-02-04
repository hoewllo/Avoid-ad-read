# EPUB广告清理工具 (C++版本)

一个用C++编写的EPUB广告内容清理工具，支持批量处理、自动解压、广告清理和重新打包。

## 📦 快速开始

### 编译程序
```bash
# 使用简单编译脚本
compile_simple.bat

# 或手动编译
mkdir bin
g++ -std=c++17 -Wall -Wextra -Iinclude -I. src/*.cpp -o bin/epub_cleaner.exe -D_WIN32
```

### 基本使用
```bash
# 清理单个EPUB文件
bin\epub_cleaner.exe -i input.epub -o output.epub

# 批量处理目录
bin\epub_cleaner.exe -I ./books -O ./cleaned_books -v
```

## 📚 详细文档

所有详细文档已移动到 `docs/` 目录：

| 文档 | 描述 |
|------|------|
| 📖 [README.md](docs/README.md) | 完整项目文档 |
| 📋 [USAGE.md](docs/USAGE.md) | 详细使用指南 |
| 🔧 [INSTALL.md](docs/INSTALL.md) | 安装说明 |
| 📊 [PROJECT_SUMMARY.md](docs/PROJECT_SUMMARY.md) | 项目技术总结 |
| ✅ [FINAL_PROJECT_STATUS.md](docs/FINAL_PROJECT_STATUS.md) | 项目完成状态 |
| 📑 [INDEX.md](docs/INDEX.md) | 文件索引 |
| 🧪 [test_epub_structure.md](docs/test_epub_structure.md) | 测试说明 |
| ⚖️ [LICENSE](docs/LICENSE) | MIT许可证 |

## 🚀 核心功能

- **自动解压EPUB文件**：将EPUB文件解压到临时目录
- **智能广告检测**：使用正则表达式匹配多种广告模式
- **批量处理**：支持单个文件或目录批量处理
- **备份机制**：自动创建.bak备份文件
- **重新打包**：清理后重新打包为干净的EPUB文件
- **参数化配置**：支持命令行参数灵活配置

## 🛠️ 项目结构

```
epub_cleaner/
├── docs/                    # 所有文档
├── src/                    # C++源代码
│   ├── main.cpp           # 主程序入口
│   ├── epub_processor.cpp # EPUB处理核心
│   ├── ad_patterns.cpp    # 广告模式管理
│   └── file_utils.cpp     # 文件操作工具
├── include/               # C++头文件
│   ├── epub_processor.h
│   ├── ad_patterns.h
│   └── file_utils.h
├── bin/                   # 编译输出目录
├── build/                 # 构建目录
├── .gitignore            # Git忽略文件
├── CMakeLists.txt        # CMake构建配置
├── build.bat             # Windows构建脚本
├── compile_simple.bat    # 简单编译脚本
├── example_patterns.txt  # 广告模式示例
└── test_integration.py   # Python集成测试
```

## 📋 命令行参数

```bash
# 基本参数
-i, --input FILE        输入EPUB文件路径
-o, --output FILE       输出EPUB文件路径
-I, --input-dir DIR     输入目录（批量处理）
-O, --output-dir DIR    输出目录（批量处理）

# 高级选项
-p, --patterns FILE     自定义广告模式文件
-v, --verbose           启用详细输出
-n, --no-backup         不创建备份文件
-h, --help              显示帮助信息
-V, --version           显示版本信息
```

## 🧪 测试

### 集成测试
```bash
# 运行Python集成测试
python test_integration.py
```

### 创建测试EPUB
参考 `docs/test_epub_structure.md` 创建测试文件。

## 📄 许可证

MIT License - 详见 [docs/LICENSE](docs/LICENSE)

## 🤝 贡献

欢迎贡献代码、报告问题或提出改进建议！

---

**项目状态**: ✅ 已完成
**版本**: v1.0.0
**最后更新**: 2024年1月26日