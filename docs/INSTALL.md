files# EPUB广告清理工具 - 安装指南

## 快速安装

### Windows用户

1. **下载预编译版本**（如果可用）：
   - 从发布页面下载 `epub_cleaner.exe`
   - 将文件放在任意目录
   - 将该目录添加到PATH环境变量（可选）

2. **从源代码编译**：
   ```bash
   # 方法1: 使用编译脚本
   compile_simple.bat
   
   # 方法2: 手动编译
   mkdir bin
   g++ -std=c++17 -Wall -Wextra -Iinclude -I. src/*.cpp -o bin/epub_cleaner.exe -D_WIN32
   ```

### Linux/macOS用户

```bash
# 安装编译依赖
sudo apt-get install g++  # Ubuntu/Debian
# 或
brew install gcc          # macOS

# 编译
mkdir -p bin
g++ -std=c++17 -Wall -Wextra -Iinclude -I. src/*.cpp -o bin/epub_cleaner

# 添加到PATH（可选）
sudo cp bin/epub_cleaner /usr/local/bin/
```

## 依赖项

### 必需依赖

1. **C++编译器**：
   - GCC/G++ 7.0+ 或 Clang 5.0+ 或 MSVC 2017+
   - 支持C++17标准

2. **标准库**：
   - C++17标准库（filesystem, regex等）

### 可选依赖

1. **ZIP处理**：
   - Windows: PowerShell 5.0+（已内置）
   - Linux: unzip和zip命令
   - macOS: 同Linux

## 验证安装

```bash
# 检查版本
epub_cleaner --version

# 显示帮助
epub_cleaner --help

# 测试简单命令
epub_cleaner -h
```

预期输出应显示版本信息和帮助文档。

## 故障排除

### 常见问题

#### 1. "命令未找到"或"不是内部或外部命令"

**解决方案**：
- 确保可执行文件在PATH中
- 或使用完整路径：`./bin/epub_cleaner`
- 或添加当前目录到PATH

#### 2. "缺少DLL"（Windows）

**解决方案**：
- 安装Visual C++ Redistributable
- 或静态链接：添加`-static`编译选项

#### 3. "无法打开文件"

**解决方案**：
- 检查文件路径是否正确
- 确保有读取权限
- 使用绝对路径

#### 4. 编译错误

**解决方案**：
- 确保使用C++17编译器
- 检查头文件路径
- 查看具体错误信息

### 编译选项

```bash
# 调试版本（带调试信息）
g++ -std=c++17 -g -Iinclude -I. src/*.cpp -o epub_cleaner_debug

# 发布版本（优化）
g++ -std=c++17 -O2 -Iinclude -I. src/*.cpp -o epub_cleaner_release

# 静态链接（Windows，避免DLL依赖）
g++ -std=c++17 -static -Iinclude -I. src/*.cpp -o epub_cleaner_static.exe
```

## 环境配置

### Windows PATH配置

1. 右键点击"此电脑" → "属性"
2. "高级系统设置" → "环境变量"
3. 在"系统变量"中找到"Path"
4. 点击"编辑" → "新建"
5. 添加epub_cleaner.exe所在目录
6. 点击"确定"保存

### Linux/macOS PATH配置

```bash
# 添加到~/.bashrc或~/.zshrc
export PATH="$PATH:/path/to/epub_cleaner"

# 重新加载配置
source ~/.bashrc
```

## 升级

### 从源代码升级

```bash
# 拉取最新代码
git pull origin main

# 重新编译
rm -rf bin
mkdir bin
g++ -std=c++17 -Wall -Wextra -Iinclude -I. src/*.cpp -o bin/epub_cleaner
```

### 预编译版本升级

直接下载新版本的可执行文件替换旧版本。

## 卸载

### Windows

1. 删除可执行文件
2. 从PATH中移除相关目录
3. 删除配置文件（如果有）

### Linux/macOS

```bash
# 删除可执行文件
sudo rm /usr/local/bin/epub_cleaner

# 或删除整个目录
rm -rf /path/to/epub_cleaner
```

## 支持的操作系统

- **Windows** 7/8/10/11 (x64)
- **Linux** Ubuntu 18.04+, CentOS 7+, 其他主流发行版
- **macOS** 10.15+

## 获取帮助

如果安装遇到问题：

1. 查看README.md中的详细说明
2. 检查错误信息
3. 确保满足所有依赖项
4. 在GitHub Issues中报告问题

## 许可证

本项目使用MIT许可证。安装和使用本软件即表示您同意许可证条款。