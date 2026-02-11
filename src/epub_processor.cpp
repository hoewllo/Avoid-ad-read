#include "epub_processor.h"
#include "ad_patterns.h"
#include "file_utils.h"
#include "zip_utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

EpubProcessor::EpubProcessor(bool verbose, bool createBackup) 
    : verbose(verbose), createBackupFiles(createBackup) {
    initializeDefaultPatterns();
    resetStats();
}

void EpubProcessor::initializeDefaultPatterns() {
    // 清空现有模式
    adPatterns.clear();
    
    // 添加内置广告模式
    vector<string> patternStrings = {
        // 模式1: GitHub项目推广广告
        R"([\s\r\n]*【[^】]*使用[^】]*项目[^】]*进行[^】]*下载[^】]*：[^】]*】[\s\r\n]*)",
        
        // 模式2: 包含github链接的广告
        R"([\s\r\n]*【[^】]*https?://[^】]*github[^】]*】[\s\r\n]*)",
        
        // 模式3: 特定项目名称
        R"([\s\r\n]*【[^】]*POf-L/Fanqie-novel-Downloader[^】]*】[\s\r\n]*)",
        
        // 模式4: 通用下载广告
        R"([\s\r\n]*【[^】]*下载[^】]*】[\s\r\n]*)",
        
        // 模式5: 特殊Unicode字符
        R"([‌᠎⁠﻿\u200b\u200c\u200d\u2060\ufeff]+)",
        
        // 模式6: 空方括号（可能包含空白字符）
        R"([\s\r\n]*【[\s]*】[\s\r\n]*)",
        
        // 模式7: 包含"项目"和"下载"的广告
        R"([\s\r\n]*【[^】]*项目[^】]*下载[^】]*】[\s\r\n]*)"
    };
    
    // 转换为正则表达式
    adPatterns = AdPatterns::createPatterns(patternStrings);
    
    if (verbose) {
        cout << "已初始化 " << adPatterns.size() << " 个默认广告模式" << endl;
    }
}

void EpubProcessor::setAdPatterns(const vector<regex>& patterns) {
    adPatterns = patterns;
    if (verbose) {
        cout << "已设置 " << patterns.size() << " 个自定义广告模式" << endl;
    }
}

void EpubProcessor::addAdPattern(const string& pattern) {
    try {
        adPatterns.emplace_back(pattern, regex::optimize | regex::icase);
        if (verbose) {
            cout << "已添加广告模式: " << pattern << endl;
        }
    } catch (const regex_error& e) {
        cerr << "错误: 无效的正则表达式模式: " << pattern << endl;
        cerr << "错误信息: " << e.what() << endl;
        stats.errors++;
    }
}

void EpubProcessor::resetStats() {
    stats = Stats{};
}

bool EpubProcessor::processFile(const fs::path& inputPath, const fs::path& outputPath) {
    if (verbose) {
        cout << "\n=== 开始处理文件 ===" << endl;
        cout << "输入文件: " << inputPath << endl;
        cout << "输出文件: " << outputPath << endl;
    }
    
    // 检查输入文件是否存在
    if (!FileUtils::fileExists(inputPath)) {
        cerr << "错误: 输入文件不存在: " << inputPath << endl;
        stats.errors++;
        return false;
    }
    
    // 验证文件扩展名
    string ext = FileUtils::getFileExtension(inputPath);
    if (ext != ".epub") {
        cerr << "警告: 文件扩展名不是.epub: " << inputPath << endl;
    }
    
    // 创建临时目录
    FileUtils::TempDirectory tempDir;
    if (!tempDir.isValid()) {
        cerr << "错误: 无法创建临时目录" << endl;
        stats.errors++;
        return false;
    }
    
    if (verbose) {
        cout << "临时目录: " << tempDir.getPath() << endl;
    }
    
    try {
        // 步骤1: 解压EPUB文件
        if (verbose) cout << "1. 解压EPUB文件..." << endl;
        if (!extractEpub(inputPath, tempDir.getPath())) {
            cerr << "错误: 解压EPUB文件失败" << endl;
            stats.errors++;
            return false;
        }
        
        // 步骤2: 清理解压后的文件
        if (verbose) cout << "2. 清理文件中的广告内容..." << endl;
        if (!cleanExtractedFiles(tempDir.getPath())) {
            cerr << "错误: 清理文件失败" << endl;
            stats.errors++;
            return false;
        }
        
        // 步骤3: 重新打包为EPUB
        if (verbose) cout << "3. 重新打包为EPUB..." << endl;
        if (!repackEpub(tempDir.getPath(), outputPath)) {
            cerr << "错误: 重新打包EPUB失败" << endl;
            stats.errors++;
            return false;
        }
        
        // 步骤4: 创建备份（如果需要）
        if (createBackupFiles) {
            if (verbose) cout << "4. 创建备份文件..." << endl;
            if (!createBackup(inputPath)) {
                cerr << "警告: 创建备份文件失败" << endl;
                stats.errors++;
            }
        }
        
        stats.filesProcessed++;
        stats.processedFiles.push_back(inputPath.string());
        
        if (verbose) {
            cout << "\n=== 文件处理完成 ===" << endl;
            cout << "输出文件: " << outputPath << endl;
            cout << "文件大小: " << FileUtils::getFileSize(outputPath) << " 字节" << endl;
            cout << "移除广告: " << stats.adsRemoved << " 处" << endl;
        }
        
        return true;
        
    } catch (const exception& e) {
        cerr << "处理文件时发生异常: " << e.what() << endl;
        stats.errors++;
        return false;
    }
}

bool EpubProcessor::processDirectory(const fs::path& inputDir, const fs::path& outputDir) {
    if (verbose) {
        cout << "\n=== 开始批量处理目录 ===" << endl;
        cout << "输入目录: " << inputDir << endl;
        cout << "输出目录: " << outputDir << endl;
    }
    
    // 确保输出目录存在
    if (!FileUtils::createDirectory(outputDir)) {
        cerr << "错误: 无法创建输出目录: " << outputDir << endl;
        return false;
    }
    
    // 查找所有EPUB文件
    vector<fs::path> epubFiles;
    try {
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".epub") {
                epubFiles.push_back(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "错误: 无法读取目录: " << e.what() << endl;
        return false;
    }
    
    if (epubFiles.empty()) {
        cout << "未找到EPUB文件" << endl;
        return true;
    }
    
    if (verbose) {
        cout << "找到 " << epubFiles.size() << " 个EPUB文件" << endl;
    }
    
    // 处理每个文件
    int successCount = 0;
    int failCount = 0;
    
    for (size_t i = 0; i < epubFiles.size(); ++i) {
        const auto& inputFile = epubFiles[i];
        
        if (verbose) {
            cout << "\n--- 处理文件 " << (i + 1) << "/" << epubFiles.size() << " ---" << endl;
            cout << "文件名: " << inputFile.filename() << endl;
        }
        
        // 生成输出文件路径
        fs::path outputFile = outputDir / inputFile.filename();
        
        // 处理文件
        if (processFile(inputFile, outputFile)) {
            successCount++;
        } else {
            failCount++;
            cerr << "文件处理失败: " << inputFile << endl;
        }
    }
    
    if (verbose) {
        cout << "\n=== 目录处理完成 ===" << endl;
        cout << "成功: " << successCount << " 个文件" << endl;
        cout << "失败: " << failCount << " 个文件" << endl;
        cout << "总共移除广告: " << stats.adsRemoved << " 处" << endl;
        if (stats.errors > 0) {
            cout << "警告: 处理过程中遇到 " << stats.errors << " 个错误" << endl;
        }
    }
    
    return failCount == 0;
}

bool EpubProcessor::extractEpub(const fs::path& epubPath, const fs::path& extractDir) {
    // 使用新的ZipUtils模块解压
    auto result = ZipUtils::extractZip(epubPath, extractDir);
    
    if (!result.success()) {
        cerr << "解压EPUB失败: " << result.message << endl;
        return false;
    }
    
    if (verbose) {
        cout << "  解压成功: " << extractDir << endl;
        
        // 列出解压的文件
        auto contents = ZipUtils::listZipContents(epubPath);
        if (!contents.empty()) {
            cout << "  包含文件: " << contents.size() << " 个" << endl;
            if (contents.size() <= 10) { // 只显示前10个文件
                for (const auto& file : contents) {
                    cout << "    - " << file << endl;
                }
            }
        }
    }
    
    return true;
}

bool EpubProcessor::cleanExtractedFiles(const fs::path& extractDir) {
    // 查找所有XHTML/HTML文件
    auto xhtmlFiles = FileUtils::findFilesRecursive(extractDir, ".xhtml");
    auto htmlFiles = FileUtils::findFilesRecursive(extractDir, ".html");
    
    // 合并文件列表
    vector<fs::path> allFiles;
    allFiles.insert(allFiles.end(), xhtmlFiles.begin(), xhtmlFiles.end());
    allFiles.insert(allFiles.end(), htmlFiles.begin(), htmlFiles.end());
    
    if (allFiles.empty()) {
        if (verbose) {
            cout << "  未找到HTML/XHTML文件" << endl;
        }
        return true;
    }
    
    if (verbose) {
        cout << "  找到 " << allFiles.size() << " 个HTML/XHTML文件需要清理" << endl;
    }
    
    // 清理每个文件
    bool allSuccess = true;
    int cleanedCount = 0;
    
    for (const auto& file : allFiles) {
        if (!cleanXhtmlFile(file)) {
            cerr << "警告: 清理文件失败: " << file << endl;
            allSuccess = false;
            stats.errors++;
        } else {
            cleanedCount++;
        }
    }
    
    if (verbose) {
        cout << "  已清理文件: " << cleanedCount << " 个" << endl;
    }
    
    return allSuccess;
}

bool EpubProcessor::repackEpub(const fs::path& extractDir, const fs::path& epubPath) {
    // 使用新的ZipUtils模块打包
    auto result = ZipUtils::createZip(extractDir, epubPath);
    
    if (!result.success()) {
        cerr << "重新打包EPUB失败: " << result.message << endl;
        return false;
    }
    
    if (verbose) {
        cout << "  打包成功: " << epubPath << endl;
        cout << "  文件大小: " << FileUtils::getFileSize(epubPath) << " 字节" << endl;
    }
    
    return true;
}

bool EpubProcessor::cleanXhtmlFile(const fs::path& filePath) {
    try {
        // 读取文件内容
        string content = FileUtils::readFileToString(filePath);
        if (content.empty()) {
            if (verbose) {
                cout << "    跳过空文件: " << filePath.filename() << endl;
            }
            return true;
        }
        
        // 检测文件编码
        // 检查XML声明中的编码属性
        string encoding = "UTF-8"; // 默认假设为UTF-8
        
        // 查找XML声明
        size_t xmlDeclStart = content.find("<?xml");
        if (xmlDeclStart != string::npos) {
            size_t xmlDeclEnd = content.find("?>", xmlDeclStart);
            if (xmlDeclEnd != string::npos) {
                string xmlDecl = content.substr(xmlDeclStart, xmlDeclEnd - xmlDeclStart + 2);
                
                // 查找encoding属性
                size_t encodingPos = xmlDecl.find("encoding=");
                if (encodingPos != string::npos) {
                    size_t quoteStart = xmlDecl.find_first_of("'\"", encodingPos + 9);
                    if (quoteStart != string::npos) {
                        size_t quoteEnd = xmlDecl.find_first_of("'\"", quoteStart + 1);
                        if (quoteEnd != string::npos) {
                            encoding = xmlDecl.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
                            
                            // 转换为大写以便比较
                            string encodingUpper = encoding;
                            transform(encodingUpper.begin(), encodingUpper.end(), encodingUpper.begin(), ::toupper);
                            
                            if (encodingUpper != "UTF-8" && encodingUpper != "UTF8") {
                                if (verbose) {
                                    cout << "    检测到编码: " << encoding << ", 将转换为UTF-8" << endl;
                                }
                                // 转换为UTF-8
                                content = FileUtils::toUtf8(content, encoding);
                            }
                        }
                    }
                }
            }
        }
        
        // 应用广告模式
        string cleanedContent = applyAdPatterns(content);
        
        // 检查是否有变化
        if (content == cleanedContent) {
            if (verbose) {
                cout << "    未发现广告内容: " << filePath.filename() << endl;
            }
            return true;
        }
        
        // 确保XML声明中的编码是UTF-8
        size_t xmlDeclStart2 = cleanedContent.find("<?xml");
        if (xmlDeclStart2 != string::npos) {
            size_t xmlDeclEnd2 = cleanedContent.find("?>", xmlDeclStart2);
            if (xmlDeclEnd2 != string::npos) {
                string xmlDecl = cleanedContent.substr(xmlDeclStart2, xmlDeclEnd2 - xmlDeclStart2 + 2);
                
                // 检查并更新编码属性
                size_t encodingPos = xmlDecl.find("encoding=");
                if (encodingPos != string::npos) {
                    // 更新为UTF-8
                    string newXmlDecl = xmlDecl;
                    size_t quoteStart = newXmlDecl.find_first_of("'\"", encodingPos + 9);
                    if (quoteStart != string::npos) {
                        size_t quoteEnd = newXmlDecl.find_first_of("'\"", quoteStart + 1);
                        if (quoteEnd != string::npos) {
                            newXmlDecl.replace(quoteStart + 1, quoteEnd - quoteStart - 1, "UTF-8");
                            cleanedContent.replace(xmlDeclStart2, xmlDeclEnd2 - xmlDeclStart2 + 2, newXmlDecl);
                        }
                    }
                } else {
                    // 如果没有编码属性，添加一个
                    size_t versionEnd = xmlDecl.find("version=");
                    if (versionEnd != string::npos) {
                        size_t versionQuoteEnd = xmlDecl.find_first_of("'\"", versionEnd + 8);
                        if (versionQuoteEnd != string::npos) {
                            versionQuoteEnd = xmlDecl.find_first_of("'\"", versionQuoteEnd + 1);
                            if (versionQuoteEnd != string::npos) {
                                string newXmlDecl = xmlDecl.substr(0, versionQuoteEnd + 1) + 
                                                    " encoding=\"UTF-8\"" + 
                                                    xmlDecl.substr(versionQuoteEnd + 1);
                                cleanedContent.replace(xmlDeclStart2, xmlDeclEnd2 - xmlDeclStart2 + 2, newXmlDecl);
                            }
                        }
                    }
                }
            }
        }
        
        // 写入清理后的内容
        if (!FileUtils::writeStringToFile(filePath, cleanedContent)) {
            cerr << "错误: 无法写入文件: " << filePath << endl;
            return false;
        }
        
        if (verbose) {
            cout << "    已清理文件: " << filePath.filename() << endl;
        }
        
        return true;
        
    } catch (const exception& e) {
        cerr << "清理文件时发生异常: " << filePath << " - " << e.what() << endl;
        return false;
    }
}

string EpubProcessor::applyAdPatterns(const string& content) {
    string result = content;
    int adsRemovedInThisFile = 0;
    
    for (const auto& pattern : adPatterns) {
        try {
            // 使用regex_replace替换匹配的内容
            string previous = result;
            result = regex_replace(result, pattern, "");
            
            // 统计移除的广告数量
            if (result != previous) {
                adsRemovedInThisFile++;
                stats.adsRemoved++;
                
                if (verbose) {
                    // 可以在这里记录更详细的信息
                }
            }
            
        } catch (const regex_error& e) {
            cerr << "正则表达式错误: " << e.what() << endl;
            stats.errors++;
        }
    }
    
    if (verbose && adsRemovedInThisFile > 0) {
        cout << "      移除广告: " << adsRemovedInThisFile << " 处" << endl;
    }
    
    return result;
}

bool EpubProcessor::createBackup(const fs::path& filePath) {
    bool success = FileUtils::createBackup(filePath, ".bak");
    
    if (verbose) {
        if (success) {
            cout << "  备份创建成功: " << filePath.string() << ".bak" << endl;
        } else {
            cout << "  备份创建失败" << endl;
        }
    }
    
    return success;
}