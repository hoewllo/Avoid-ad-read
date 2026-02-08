@echo off
setlocal enabledelayedexpansion

echo ========================================
echo EPUB广告清理工具 - 重构版本测试
echo ========================================
echo.

REM 复制可执行文件到当前目录
if exist "build\bin\epub_cleaner.exe" (
    copy "build\bin\epub_cleaner.exe" .
    echo ✓ 复制可执行文件
) else (
    echo ✗ 错误: 未找到可执行文件
    pause
    exit /b 1
)

echo.
echo === 测试1: 显示帮助信息 ===
epub_cleaner.exe --help
if errorlevel 1 (
    echo ✗ 测试1失败
) else (
    echo ✓ 测试1通过
)

echo.
echo === 测试2: 显示版本信息 ===
epub_cleaner.exe --version
if errorlevel 1 (
    echo ✗ 测试2失败
) else (
    echo ✓ 测试2通过
)

echo.
echo === 测试3: 列出内置广告模式 ===
epub_cleaner.exe --list-patterns
if errorlevel 1 (
    echo ✗ 测试3失败
) else (
    echo ✓ 测试3通过
)

echo.
echo === 测试4: 创建测试EPUB文件 ===
REM 创建简单的测试EPUB文件
mkdir test_epub 2>nul
cd test_epub

echo application/epub+zip > mimetype

mkdir META-INF 2>nul
echo ^<?xml version="1.0"?^> > META-INF\container.xml
echo ^<container version="1.0" xmlns="urn:oasis:names:tc:opendocument:xmlns:container"^> >> META-INF\container.xml
echo   ^<rootfiles^> >> META-INF\container.xml
echo     ^<rootfile full-path="EPUB/content.opf" media-type="application/oebps-package+xml"/^> >> META-INF\container.xml
echo   ^</rootfiles^> >> META-INF\container.xml
echo ^</container^> >> META-INF\container.xml

mkdir EPUB 2>nul
echo ^<?xml version="1.0" encoding="UTF-8"?^> > EPUB\content.opf
echo ^<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="uid"^> >> EPUB\content.opf
echo   ^<metadata xmlns:dc="http://purl.org/dc/elements/1.1/"^> >> EPUB\content.opf
echo     ^<dc:identifier id="uid"^>test-book-001^</dc:identifier^> >> EPUB\content.opf
echo     ^<dc:title^>测试书籍^</dc:title^> >> EPUB\content.opf
echo     ^<dc:language^>zh-CN^</dc:language^> >> EPUB\content.opf
echo     ^<dc:creator^>测试作者^</dc:creator^> >> EPUB\content.opf
echo   ^</metadata^> >> EPUB\content.opf
echo   ^<manifest^> >> EPUB\content.opf
echo     ^<item id="chapter1" href="chapter1.xhtml" media-type="application/xhtml+xml"/^> >> EPUB\content.opf
echo   ^</manifest^> >> EPUB\content.opf
echo   ^<spine^> >> EPUB\content.opf
echo     ^<itemref idref="chapter1"/^> >> EPUB\content.opf
echo   ^</spine^> >> EPUB\content.opf
echo ^</package^> >> EPUB\content.opf

echo ^<?xml version="1.0" encoding="UTF-8"?^> > EPUB\chapter1.xhtml
echo ^<html xmlns="http://www.w3.org/1999/xhtml"^> >> EPUB\chapter1.xhtml
echo ^<head^> >> EPUB\chapter1.xhtml
echo   ^<title^>测试章节^</title^> >> EPUB\chapter1.xhtml
echo ^</head^> >> EPUB\chapter1.xhtml
echo ^<body^> >> EPUB\chapter1.xhtml
echo   ^<h1^>第一章 测试^</h1^> >> EPUB\chapter1.xhtml
echo   ^<p^>这是正常的文本内容。^</p^> >> EPUB\chapter1.xhtml
echo   ^<p^>【使用POf-L/Fanqie-novel-Downloader项目进行下载：https://github.com/POf-L/Fanqie-novel-Downloader】^</p^> >> EPUB\chapter1.xhtml
echo   ^<p^>继续正常的内容。^</p^> >> EPUB\chapter1.xhtml
echo   ^<p^>【包含特殊字符的广告᠎⁠﻿】^</p^> >> EPUB\chapter1.xhtml
echo   ^<p^>【另一个广告：下载工具】^</p^> >> EPUB\chapter1.xhtml
echo   ^<p^>更多正常文本。^</p^> >> EPUB\chapter1.xhtml
echo ^</body^> >> EPUB\chapter1.xhtml
echo ^</html^> >> EPUB\chapter1.xhtml

cd ..

REM 创建ZIP文件（EPUB）
if exist test.epub del test.epub
powershell -Command "Compress-Archive -Path 'test_epub/*' -DestinationPath 'test.epub' -Force"
if exist test.epub (
    echo ✓ 创建测试EPUB文件
    echo   文件大小: %~z0 字节
) else (
    echo ✗ 创建测试EPUB文件失败
    goto cleanup
)

echo.
echo === 测试5: 清理EPUB文件 ===
epub_cleaner.exe -i test.epub -o cleaned.epub -v
if errorlevel 1 (
    echo ✗ 测试5失败
) else (
    echo ✓ 测试5通过
    if exist cleaned.epub (
        echo   输出文件大小: %~z0 字节
    )
)

:cleanup
echo.
echo === 清理测试文件 ===
if exist test.epub (
    del test.epub
    echo ✓ 删除测试EPUB文件
)
if exist cleaned.epub (
    del cleaned.epub
    echo ✓ 删除清理后的EPUB文件
)
if exist test_epub (
    rmdir /s /q test_epub
    echo ✓ 删除测试目录
)
if exist epub_cleaner.exe (
    del epub_cleaner.exe
    echo ✓ 删除可执行文件
)

echo.
echo ========================================
echo 测试完成!
echo ========================================
pause