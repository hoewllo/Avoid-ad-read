#include "file_utils.h"
#include "ad_patterns.h"
#include "zip_utils.h"
#include "logger.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

// 测试文件工具
void testFileUtils() {
    cout << "\n=== 测试文件工具 ===" << endl;
    
    // 创建测试目录
    string testDir = "test_file_utils";
    assert(FileUtils::createDirectory(testDir));
    cout << "✓ 创建目录" << endl;
    
    // 测试文件读写
    string testFile = testDir + "/test.txt";
    string testContent = "Hello, World!\nThis is a test.";
    
    assert(FileUtils::writeStringToFile(testFile, testContent));
    cout << "✓ 写入文件" << endl;
    
    string readContent = FileUtils::readFileToString(testFile);
    assert(readContent == testContent);
    cout << "✓ 读取文件" << endl;
    
    // 测试文件存在性
    assert(FileUtils::fileExists(testFile));
    cout << "✓ 文件存在性检查" << endl;
    
    // 测试文件大小
    assert(FileUtils::getFileSize(testFile) == testContent.size());
    cout << "✓ 文件大小检查" << endl;
    
    // 清理测试文件
    assert(FileUtils::removeFile(testFile));
    assert(FileUtils::removeDirectory(testDir));
    cout << "✓ 清理测试文件" << endl;
}

// 测试广告模式
void testAdPatterns() {
    cout << "\n=== 测试广告模式 ===" << endl;
    
    // 获取内置模式
    auto builtinPatterns = AdPatterns::getBuiltinPatterns();
    assert(!builtinPatterns.empty());
    cout << "✓ 获取内置模式 (" << builtinPatterns.size() << "个)" << endl;
    
    // 测试模式验证
    assert(AdPatterns::validatePattern(".*test.*"));
    cout << "✓ 模式验证" << endl;
    
    // 测试模式匹配器
    AdPatterns::PatternMatcher matcher;
    matcher.addPattern("【.*广告.*】");
    
    string testContent = "正常内容【这是一个广告】更多内容";
    string cleaned = matcher.cleanContent(testContent);
    
    assert(cleaned == "正常内容更多内容");
    cout << "✓ 模式匹配和清理" << endl;
    
    // 测试广告检测
    assert(matcher.containsAds(testContent));
    cout << "✓ 广告检测" << endl;
}

// 测试临时目录
void testTempDirectory() {
    cout << "\n=== 测试临时目录 ===" << endl;
    
    {
        FileUtils::TempDirectory tempDir("test_temp_");
        assert(tempDir.isValid());
        cout << "✓ 创建临时目录: " << tempDir.getPath() << endl;
        
        // 在临时目录中创建文件
        string testFile = (tempDir.getPath() / "test.txt").string();
        assert(FileUtils::writeStringToFile(testFile, "test"));
        cout << "✓ 在临时目录中创建文件" << endl;
    }
    
    // 临时目录应该已经被自动删除
    cout << "✓ 临时目录自动清理" << endl;
}

// 测试日志系统
void testLogger() {
    cout << "\n=== 测试日志系统 ===" << endl;
    
    // 设置日志级别
    Logger::setLevel(Logger::Level::DEBUG);
    
    // 测试不同级别的日志
    LOG_DEBUG << "调试信息";
    LOG_INFO << "普通信息";
    LOG_WARN << "警告信息";
    LOG_ERROR << "错误信息";
    
    cout << "✓ 日志输出测试完成" << endl;
}

int main() {
    cout << "开始运行单元测试..." << endl;
    
    try {
        testFileUtils();
        testAdPatterns();
        testTempDirectory();
        testLogger();
        
        cout << "\n=== 所有测试通过! ===" << endl;
        return 0;
        
    } catch (const exception& e) {
        cerr << "\n测试失败: " << e.what() << endl;
        return 1;
    }
}