# EPUB文件结构测试说明

## 标准EPUB文件结构

一个标准的EPUB文件（本质上是ZIP文件）包含以下结构：

```
example.epub (ZIP文件)
├── mimetype                 # 必须为第一个文件，内容：application/epub+zip
├── META-INF/
│   └── container.xml       # 指向OPF文件
├── OEBPS/                  # 内容目录（名称可能不同）
│   ├── content.opf         # 包文档，列出所有资源
│   ├── toc.ncx             # 导航控制文件
│   ├── Styles/
│   │   └── style.css      # 样式文件
│   └── Text/
│       ├── chapter1.xhtml # 章节内容
│       ├── chapter2.xhtml
│       └── ...
└── images/                 # 图片目录
    └── cover.jpg
```

## 创建测试EPUB文件

### 方法1：手动创建ZIP文件

1. 创建以下目录结构：
```
test_book/
├── mimetype
├── META-INF/
│   └── container.xml
└── EPUB/
    ├── content.opf
    ├── toc.ncx
    └── chapter1.xhtml
```

2. 创建各文件内容：

**mimetype** (无扩展名，纯文本)：
```
application/epub+zip
```

**META-INF/container.xml**：
```xml
<?xml version="1.0"?>
<container version="1.0" xmlns="urn:oasis:names:tc:opendocument:xmlns:container">
  <rootfiles>
    <rootfile full-path="EPUB/content.opf" media-type="application/oebps-package+xml"/>
  </rootfiles>
</container>
```

**EPUB/content.opf**：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="uid">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/">
    <dc:identifier id="uid">test-book-001</dc:identifier>
    <dc:title>测试书籍</dc:title>
    <dc:language>zh-CN</dc:language>
    <dc:creator>测试作者</dc:creator>
  </metadata>
  <manifest>
    <item id="toc" href="toc.ncx" media-type="application/x-dtbncx+xml"/>
    <item id="chapter1" href="chapter1.xhtml" media-type="application/xhtml+xml"/>
  </manifest>
  <spine>
    <itemref idref="chapter1"/>
  </spine>
</package>
```

**EPUB/chapter1.xhtml** (包含广告内容用于测试)：
```html
<?xml version="1.0" encoding="UTF-8"?>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>第一章</title>
</head>
<body>
  <h1>第一章 测试章节</h1>
  <p>这是正常的文本内容。</p>
  
  <p>【使用POf-L/Fanqie-novel-Downloader项目进行下载：https://github.com/POf-L/Fanqie-novel-Downloader】</p>
  
  <p>继续正常的内容。</p>
  
  <p>【包含特殊字符的广告᠎⁠﻿】</p>
  
  <p>更多正常文本。</p>
</body>
</html>
```

3. 使用ZIP工具创建EPUB文件：
```bash
# 注意：mimetype必须是ZIP文件中的第一个条目，且不能压缩
cd test_book
zip -0Xq test.epub mimetype
zip -Xr9Dq test.epub META-INF EPUB
```

### 方法2：使用现有Python脚本转换

可以使用之前的Python清理脚本处理现有的EPUB文件进行测试。

## 测试C++程序

### 编译程序
```bash
# 使用CMake
mkdir build
cd build
cmake ..
cmake --build .
```

### 运行测试
```bash
# 清理单个文件
./epub_cleaner -i test.epub -o cleaned.epub -v

# 批量处理目录
./epub_cleaner -I ./input_books -O ./cleaned_books -v

# 使用自定义模式文件
./epub_cleaner -i test.epub -p custom_patterns.txt -v
```

## 验证清理结果

1. 解压清理后的EPUB文件：
```bash
unzip cleaned.epub -d cleaned_content
```

2. 检查XHTML文件：
```bash
grep -r "【" cleaned_content/  # 应该没有输出
grep -r "github" cleaned_content/  # 应该没有输出
```

3. 手动查看文件内容确认广告已被移除。

## 注意事项

1. **ZIP库依赖**：程序需要zlib或libzip库来处理ZIP压缩
2. **编码问题**：确保正确处理UTF-8编码
3. **临时文件**：程序会创建临时目录，确保有足够的磁盘空间
4. **备份文件**：默认会创建.bak备份文件，确保有写入权限

## 故障排除

### 常见问题

1. **无法解压EPUB文件**
   - 检查zlib/libzip库是否正确安装
   - 检查文件权限

2. **广告清理不彻底**
   - 检查广告模式是否匹配
   - 添加自定义模式到配置文件中

3. **程序崩溃**
   - 检查内存使用
   - 确保输入文件格式正确

### 调试模式

使用`-v`参数启用详细输出，查看处理过程：
```bash
./epub_cleaner -i test.epub -o cleaned.epub -v
```