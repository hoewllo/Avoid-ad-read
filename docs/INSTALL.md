# EPUB Ad Cleaner - Installation Guide

## Quick Installation

### Windows Users

1. **Download Precompiled Version** (if available):
   - Download `epub_cleaner.exe` from the release page
   - Place the file in any directory
   - Add the directory to PATH environment variable (optional)

2. **Compile from Source**:
   ```bash
   # Method 1: Use compilation script
   tools/build-tool/compile_simple.bat
   
   # Method 2: Manual compilation
   mkdir bin
   g++ -std=c++17 -Wall -Wextra -Iinclude -I. src/*.cpp -o bin/epub_cleaner.exe -D_WIN32
   ```

### Linux/macOS Users

```bash
# Install compilation dependencies
sudo apt-get install g++  # Ubuntu/Debian
# or
brew install gcc          # macOS

# Compile
mkdir -p bin
g++ -std=c++17 -Wall -Wextra -Iinclude -I. src/*.cpp -o bin/epub_cleaner

# Add to PATH (optional)
sudo cp bin/epub_cleaner /usr/local/bin/
```

## Dependencies

### Required Dependencies

1. **C++ Compiler**:
   - GCC/G++ 7.0+ or Clang 5.0+ or MSVC 2017+
   - Support for C++17 standard

2. **Standard Library**:
   - C++17 standard library (filesystem, regex, etc.)

### Optional Dependencies

1. **ZIP Processing**:
   - Windows: PowerShell 5.0+ (built-in)
   - Linux: unzip and zip commands
   - macOS: Same as Linux

## Verify Installation

```bash
# Check version
epub_cleaner --version

# Show help
epub_cleaner --help

# Test simple command
epub_cleaner -h
```

Expected output should show version information and help documentation.

## Troubleshooting

### Common Issues

#### 1. "Command not found" or "Not an internal or external command"

**Solution**:
- Ensure the executable is in PATH
- Or use full path: `./bin/epub_cleaner`
- Or add current directory to PATH

#### 2. "Missing DLL" (Windows)

**Solution**:
- Install Visual C++ Redistributable
- Or static linking: Add `-static` compilation option

#### 3. "Cannot open file"

**Solution**:
- Check if file path is correct
- Ensure read permissions
- Use absolute path

#### 4. Compilation Errors

**Solution**:
- Ensure using C++17 compiler
- Check header file paths
- View specific error messages

### Compilation Options

```bash
# Debug version (with debug information)
g++ -std=c++17 -g -Iinclude -I. src/*.cpp -o epub_cleaner_debug

# Release version (optimized)
g++ -std=c++17 -O2 -Iinclude -I. src/*.cpp -o epub_cleaner_release

# Static linking (Windows, avoid DLL dependencies)
g++ -std=c++17 -static -Iinclude -I. src/*.cpp -o epub_cleaner_static.exe
```

## Environment Configuration

### Windows PATH Configuration

1. Right-click "This PC" → "Properties"
2. "Advanced system settings" → "Environment Variables"
3. Find "Path" in "System variables"
4. Click "Edit" → "New"
5. Add directory containing epub_cleaner.exe
6. Click "OK" to save

### Linux/macOS PATH Configuration

```bash
# Add to ~/.bashrc or ~/.zshrc
export PATH="$PATH:/path/to/epub_cleaner"

# Reload configuration
source ~/.bashrc
```

## Upgrading

### Upgrade from Source

```bash
# Pull latest code
git pull origin main

# Recompile
rm -rf bin
mkdir bin
g++ -std=c++17 -Wall -Wextra -Iinclude -I. src/*.cpp -o bin/epub_cleaner
```

### Precompiled Version Upgrade

Directly download new version executable to replace old version.

## Uninstallation

### Windows

1. Delete executable file
2. Remove related directory from PATH
3. Delete configuration files (if any)

### Linux/macOS

```bash
# Delete executable file
sudo rm /usr/local/bin/epub_cleaner

# Or delete entire directory
rm -rf /path/to/epub_cleaner
```

## Supported Operating Systems

- **Windows** 7/8/10/11 (x64)
- **Linux** Ubuntu 18.04+, CentOS 7+, other mainstream distributions
- **macOS** 10.15+

## Getting Help

If installation encounters problems:

1. Check detailed instructions in README.md
2. Check error messages
3. Ensure all dependencies are met
4. Report issues in GitHub Issues

## License

This project uses the MIT License. Installing and using this software indicates your agreement to the license terms.