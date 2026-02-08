# EPUB Ad Cleaner

A C++ EPUB ad content cleaning tool that supports batch processing, automatic extraction, ad cleaning, and repackaging.

## Features

- **Automatic EPUB extraction**: Extract EPUB files to temporary directories
- **Intelligent ad detection**: Use regex patterns to match various ad formats
- **Batch processing**: Support single file or directory batch processing
- **Backup mechanism**: Automatically create .bak backup files
- **Repackaging**: Clean and repackage into clean EPUB files
- **Parameterized configuration**: Flexible command-line parameter configuration

## Ad Pattern Recognition

The tool can identify and clean the following types of ad content:

1. **GitHub project promotion ads**:
   - Format: `【Use xxx project for download: https://github.com/xxx】`
   - Example: `【Use POf-L/Fanqie-novel-Downloader project for download: https://github.com/POf-L/Fanqie-novel-Downloader】`

2. **Special Unicode characters**:
   - Zero-width characters: `\u200b`, `\u200c`, `\u200d`, `\u2060`, `\ufeff`
   - Other special characters: `‌`, `᠎`, `⁠`, `﻿`

3. **General ad patterns**:
   - Square bracket content containing keywords like "download", "project", "use"

## Installation and Compilation

### Dependencies
- C++17 or higher
- zlib (for EPUB compression/decompression)
- libzip (optional, for ZIP operations)
- Standard library with regex support

### Compilation Commands
```bash
# Using CMake
mkdir build
cd build
cmake ..
cmake --build .

# Or directly using g++
g++ -std=c++17 -o epub_cleaner main.cpp -lz
```

## Usage

### Basic Usage
```bash
# Clean a single EPUB file
epub_cleaner --input book.epub --output clean_book.epub

# Batch process all EPUB files in a directory
epub_cleaner --input-dir ./books --output-dir ./cleaned_books

# Enable verbose output
epub_cleaner --input book.epub --output clean_book.epub --verbose

# Do not create backup files
epub_cleaner --input book.epub --output clean_book.epub --no-backup
```

### Command Line Arguments

| Argument | Short | Description | Default |
|----------|-------|-------------|---------|
| `--input` | `-i` | Input EPUB file path | None |
| `--output` | `-o` | Output EPUB file path | Input file with "_clean" suffix |
| `--input-dir` | `-I` | Input directory (batch processing) | None |
| `--output-dir` | `-O` | Output directory (batch processing) | Input directory with "_cleaned" suffix |
| `--verbose` | `-v` | Enable verbose output | false |
| `--no-backup` | `-n` | Do not create backup files | false |
| `--patterns` | `-p` | Custom ad pattern file | Use built-in patterns |
| `--help` | `-h` | Show help information | None |

## Project Structure

```
epub_cleaner/
├── src/
│   ├── main.cpp              # Main program entry
│   ├── epub_processor.cpp    # EPUB processing core logic
│   ├── epub_processor.h
│   ├── ad_patterns.cpp       # Ad pattern definitions
│   ├── ad_patterns.h
│   ├── file_utils.cpp        # File utility functions
│   └── file_utils.h
├── include/                  # Header files
├── tests/                   # Test files
├── CMakeLists.txt           # CMake build configuration
└── README.md               # Project documentation
```

## Technical Implementation

### 1. EPUB File Structure Processing
- Use zlib for ZIP compression/decompression
- Parse EPUB's mimetype and container.xml
- Traverse OPF files to get all XHTML content files

### 2. Ad Cleaning Algorithm
- Multi-pattern matching based on regular expressions
- Support for Unicode character processing
- Preserve original file format and encoding

### 3. Error Handling
- Comprehensive error checking and exception handling
- Detailed error message output
- Clean up temporary files to prevent resource leaks

## Examples

### Before Cleaning (chapter_39.xhtml fragment)
```html
<p>Novel content...</p>
<p>【Use POf-L/Fanqie-novel-Downloader project for download: https://github.com/POf-L/Fanqie-novel-Downloader】</p>
<p>Continue novel content...</p>
```

### After Cleaning
```html
<p>Novel content...</p>
<p>Continue novel content...</p>
```

## Performance Optimization

- Use memory-mapped files to improve large file processing speed
- Multi-threading support for batch processing
- Incremental updates to avoid full file rewriting

## License

MIT License

## Contribution Guidelines

1. Fork the project
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## Issue Reporting

If you encounter problems or have improvement suggestions, please submit them in GitHub Issues.
```

## Changelog

### v1.0.0 (2024-01-26)
- Initial version release
- Support basic EPUB ad cleaning functionality
- Implement command-line parameter processing
- Add backup mechanism