# EPUB Ad Cleaner - Usage Guide (v1.1.0)

## Quick Start

### 1. Compile the Program

#### Windows
```bash
# Method 1: Use the simple compilation script
tools/build-tool/compile_simple.bat

# Method 2: Use the full build script
tools/build-tool/build.bat

# Method 3: Manual CMake
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

### 2. Basic Usage

```bash
# Clean a single EPUB file
epub_cleaner -i input.epub -o output.epub

# Batch process a directory
epub_cleaner -I ./input_dir -O ./output_dir

# Enable verbose output
epub_cleaner -i input.epub -v

# List built-in ad patterns
epub_cleaner --list-patterns

# Enable debug mode
epub_cleaner -i input.epub -d

# Quiet mode (only errors)
epub_cleaner -i input.epub -q
```

## Detailed Function Description

### Ad Patterns

The tool has 7 built-in ad patterns that can identify the following types of ad content:

1. **GitHub project promotion**: Contains GitHub links and project names
2. **Download ads**: Contains the keyword \"download\"
3. **Special characters**: Zero-width characters and other special Unicode characters
4. **Empty brackets**: Possibly hidden ad markers

### Custom Ad Patterns

You can create custom ad pattern files:

```bash
# Create pattern file custom_patterns.txt
# One regular expression per line

# Example: Match specific ads
【.*ad.*】
【.*promotion.*】

# Use custom patterns
epub_cleaner -i input.epub -p custom_patterns.txt
```

### Backup Mechanism

By default, the program creates .bak backup files for each processed file.

```bash
# Disable backup
epub_cleaner -i input.epub -n

# Backup file location: Same directory as original file
input.epub      # Original file
input.epub.bak  # Backup file
```

## New Features in v1.1.0

### Enhanced ZIP Processing
- **New zip_utils module**: Improved ZIP file handling with platform-specific implementations
- **Better error handling**: More detailed error messages for ZIP operations
- **Fallback mechanisms**: Multiple fallback methods for ZIP extraction/compression

### Advanced Logging System
- **Multi-level logging**: DEBUG, INFO, WARN, ERROR, FATAL levels
- **Configurable output**: Control log verbosity with -v, -d, -q flags
- **Timestamp support**: Optional timestamps in log messages

### Improved File Utilities
- **Enhanced file operations**: Better file and directory management
- **Cross-platform compatibility**: Improved support for Windows and Unix systems
- **Error recovery**: Better handling of file system errors

## Advanced Usage

### Batch Processing Script

Create a batch script `clean_all.bat` (Windows):
```batch
@echo off
setlocal

set INPUT_DIR=books
set OUTPUT_DIR=cleaned_books
set LOG_FILE=cleanup.log

echo Starting batch EPUB cleaning...
echo Input directory: %INPUT_DIR%
echo Output directory: %OUTPUT_DIR%
echo.

if not exist \"%OUTPUT_DIR%\" mkdir \"%OUTPUT_DIR%\"

epub_cleaner -I \"%INPUT_DIR%\" -O \"%OUTPUT_DIR%\" -v > \"%LOG_FILE%\" 2>&1

if errorlevel 1 (
    echo Processing failed, please check log file: %LOG_FILE%
) else (
    echo Processing completed! Log file: %LOG_FILE%
)

pause
```

### Integration into Workflow

You can integrate this tool into an ebook processing pipeline:

```bash
#!/bin/bash
# ebook_pipeline.sh

INPUT=\"$1\"
OUTPUT=\"${INPUT%.epub}_clean.epub\"
LOG=\"clean_$(date +%Y%m%d_%H%M%S).log\"

# Step 1: Clean ads
epub_cleaner -i \"$INPUT\" -o \"$OUTPUT\" -v > \"$LOG\" 2>&1

if [ $? -eq 0 ]; then
    echo \"Ad cleaning completed: $OUTPUT\"
    
    # Step 2: Optional additional processing
    # ...
    
    # Step 3: Verify results
    echo \"Verifying cleaning results...\"
    grep -q \"【\" \"$OUTPUT\" && echo \"Warning: Possible ad residue\" || echo \"Verification passed\"
else
    echo \"Cleaning failed, please check log: $LOG\"
    exit 1
fi
```

## Performance Optimization

### Processing Large Files

For large EPUBs containing many XHTML files:

```bash
# Use less memory (process file by file)
epub_cleaner -i large_book.epub --no-backup

# Limit concurrency during batch processing (if needed)
# Note: Current version is single-threaded
```

### Disk Space

- Temporary directory: The program creates temporary directories, requires sufficient disk space
- Backup files: Each file creates a backup, ensure enough space

## Troubleshooting

### Common Errors

1. **\"Cannot open file\"**
   - Check if the file path is correct
   - Check file permissions
   
2. **\"Regular expression error\"**
   - Check regex syntax in custom pattern files
   - Try simplifying patterns
   
3. **\"Insufficient memory\"**
   - May occur when processing very large files
   - Try processing in batches

4. **\"ZIP extraction failed\"**
   - Ensure system has ZIP utilities installed
   - Check file permissions
   - Try running with administrator privileges

### Debugging Tips

1. **Enable verbose output**: Use `-v` parameter
2. **Enable debug mode**: Use `-d` parameter for detailed logs
3. **Check temporary files**: The program creates files in the system temp directory
4. **View logs**: Redirect output to log files

## Examples

### Example 1: Basic Cleaning

```bash
# Input file: book.epub
# Output file: book_clean.epub
# Create backup: Yes
# Verbose output: No

epub_cleaner -i book.epub
```

### Example 2: Batch Processing with Logging

```bash
# Process all EPUB files in directory
# Output to new directory
# Record detailed logs

epub_cleaner -I ./my_books -O ./cleaned_books -v > cleanup.log
```

### Example 3: Custom Pattern Cleaning with Debug

```bash
# Use custom ad patterns
# Do not create backup
# Enable debug output

epub_cleaner -i book.epub -o clean.epub -p my_patterns.txt -n -d
```

### Example 4: Quiet Mode Processing

```bash
# Process in quiet mode (only show errors)
# Useful for automated scripts

epub_cleaner -i book.epub -o clean.epub -q
```

## Updates and Maintenance

### Updating Ad Patterns

1. Edit the `example_patterns.txt` file
2. Add new regular expression patterns
3. Use `-p` parameter to specify pattern file

### Reporting Issues

If you find incomplete ad cleaning or program issues:

1. Save the original EPUB file
2. Record the commands and parameters used
3. Provide error messages or logs
4. Report specific ad content patterns

## Testing

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

## License

This project uses the MIT License. See LICENSE file for details.

## Contributing

Welcome to contribute code, report issues, or suggest improvements!

---

**Project Status**: ✅ Completed and Refactored  
**Version**: v1.1.0  
**Last Updated**: January 26, 2024  
**Build Tools Location**: tools/build-tool/  
**Test Tools Location**: tools/test/