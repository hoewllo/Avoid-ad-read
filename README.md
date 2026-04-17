# EPUB Ad Cleaner (C++ Version)

A C++ EPUB ad content cleaning tool that supports batch processing, automatic extraction, ad cleaning, and repackaging.

## 📦 Quick Start

### Compile the Program

#### Using Unified Build Script (Recommended)
```bash
# Windows (PowerShell required)
.\build.bat

# With options
.\build.bat -Preset debug -Test
.\build.bat -Clean
.\build.bat -Help

# Or use the PowerShell script directly
powershell -ExecutionPolicy Bypass -File tools\scripts\build\build.ps1
```

#### Using CMake Presets
```bash
# Configure with default preset
cmake --preset default

# Build
cmake --build --preset default

# Run tests
ctest --preset default
```

#### Legacy Build Scripts
```bash
# Simple compilation
tools/build-tool/compile_simple.bat

# Full build script
tools/build-tool/build.bat

# Manual compilation
mkdir bin
g++ -std=c++17 -Wall -Wextra -Iinclude -I. src/*.cpp -o bin/epub_cleaner.exe -D_WIN32
```

### Basic Usage
```bash
# Clean a single EPUB file
bin\epub_cleaner.exe -i input.epub -o output.epub

# Batch process a directory
bin\epub_cleaner.exe -I ./books -O ./cleaned_books -v

# List built-in ad patterns
bin\epub_cleaner.exe --list-patterns
```

## 🏗️ Project Structure

The project follows a clean and organized structure:

```
epub-ad-cleaner/
├── CMakeLists.txt          # Main CMake configuration
├── CMakePresets.json       # CMake preset configurations
├── config/                 # Project configuration files
├── docs/                   # Documentation (including PROJECT_STRUCTURE.md)
├── cmake/                 # CMake modules and toolchains
├── include/               # Header files
├── src/                   # Source code
└── tools/                 # Tools, scripts and tests
    ├── build-tool/        # Build tools
    ├── scripts/           # Build and utility scripts (including build.bat)
    ├── test/             # Test code and scripts
    └── example_patterns.txt
```

For detailed structure information, see [docs/PROJECT_STRUCTURE.md](docs/PROJECT_STRUCTURE.md).

## 📚 Documentation

All detailed documentation is available in the `docs/` directory:

| Document | Description | Language |
|----------|-------------|----------|
| 📖 [README](docs/README) | Complete project documentation | English |
| 📖 [README_ZH](docs/README_ZH) | 完整项目文档 | Chinese |
| 📋 [USAGE](docs/USAGE) | Detailed usage guide | English |
| 📋 [USAGE_ZH](docs/USAGE_ZH) | 详细使用指南 | Chinese |
| 🔧 [INSTALL](docs/INSTALL) | Installation instructions | English |
| 🔧 [INSTALL_ZH](docs/INSTALL_ZH) | 安装说明 | Chinese |
| 📊 [PROJECT_SUMMARY](docs/PROJECT_SUMMARY) | Technical project summary | English |
| 📊 [PROJECT_SUMMARY_ZH](docs/PROJECT_SUMMARY_ZH) | 项目技术总结 | Chinese |
| ✅ [FINAL_PROJECT_STATUS](docs/FINAL_PROJECT_STATUS) | Project completion status | English |
| ✅ [FINAL_PROJECT_STATUS_ZH](docs/FINAL_PROJECT_STATUS_ZH) | 项目完成状态 | Chinese |
| 🧪 [TEST_EPUB_STRUCTURE](docs/TEST_EPUB_STRUCTURE) | Test documentation | English |
| 🧪 [TEST_EPUB_STRUCTURE_ZH](docs/TEST_EPUB_STRUCTURE_ZH) | 测试说明 | Chinese |

## 🚀 Core Features

- **Automatic EPUB extraction**: Extract EPUB files to temporary directories
- **Intelligent ad detection**: Use regex patterns to match various ad formats
- **Batch processing**: Support single file or directory batch processing
- **Backup mechanism**: Automatically create .bak backup files
- **Repackaging**: Clean and repackage into clean EPUB files
- **Parameterized configuration**: Flexible command-line parameter configuration
- **Detailed logging**: Support DEBUG, INFO, WARN, ERROR log levels

## 🛠️ Project Structure

```
epub_cleaner/
├── docs/                    # All documentation (English/Chinese)
├── src/                    # C++ source code
│   ├── main.cpp           # Main program entry
│   ├── epub_processor.cpp # EPUB processing core
│   ├── ad_patterns.cpp    # Ad pattern management
│   ├── file_utils.cpp     # File operation utilities
│   ├── zip_utils_impl.cpp # ZIP file processing implementation
│   ├── zlib_utils.cpp     # zlib compression utilities
│   └── logger.cpp         # Logging system
├── include/               # C++ header files
│   ├── epub_processor.h
│   ├── ad_patterns.h
│   ├── file_utils.h
│   ├── zip_utils.h
│   └── logger.h
├── tools/                 # Tool scripts
│   ├── build-tool/       # Build tools
│   │   ├── build.bat     # Full build script
│   │   └── compile_simple.bat # Simple compilation script
│   └── test/             # Testing tools
│       ├── test_main.cpp # Unit tests
│       └── test_refactored.bat # Refactored version tests
├── bin/                   # Compiled output directory
├── build/                 # CMake build directory
├── .gitignore            # Git ignore file
├── CMakeLists.txt        # CMake build configuration
├── example_patterns.txt  # Ad pattern examples
└── LICENSE               # MIT License
```

## 📋 Command Line Arguments

```bash
# Basic parameters
-i, --input FILE        Input EPUB file path
-o, --output FILE       Output EPUB file path
-I, --input-dir DIR     Input directory (batch processing)
-O, --output-dir DIR    Output directory (batch processing)

# Ad pattern options
-p, --patterns FILE     Custom ad pattern file
--list-patterns        List all built-in ad patterns

# Logging and output options
-v, --verbose           Enable verbose output
-q, --quiet             Silent mode, only show errors
-d, --debug             Enable debug output

# Other options
-n, --no-backup         Do not create backup files
-h, --help              Show help information
-V, --version           Show version information
```

## 🧪 Testing

### Unit Tests
```bash
# Compile and run unit tests
cd tools/test
g++ -std=c++17 -I../../include -I../.. test_main.cpp ../../src/file_utils.cpp ../../src/ad_patterns.cpp ../../src/logger.cpp -o test_runner
test_runner.exe
```

### Integration Tests
```bash
# Run refactored version tests
tools/test/test_refactored.bat
```

### Create Test EPUB
Refer to `docs/TEST_EPUB_STRUCTURE` to create test files.

## 📄 License

MIT License - See [LICENSE](LICENSE)

## 🤝 Contributing

Welcome to contribute code, report issues, or suggest improvements!

---

**Project Status**: ✅ Completed and Refactored
**Version**: v1.1.0
**Last Updated**: January 26, 2024
**Build Tools Location**: tools/build-tool/
**Test Tools Location**: tools/test/