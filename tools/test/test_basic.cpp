#include <iostream>
#include <cassert>
#include "../include/file_utils.h"
#include "../include/logger.h"

// 简单的测试函数
void test_file_utils_basic() {
    std::cout << "Testing FileUtils basic functions..." << std::endl;
    
    // 测试路径规范化
    std::string path1 = "test\\path\\to\\file.txt";
    std::string normalized = FileUtils::normalizePath(path1);
    std::cout << "Normalized path: " << normalized << std::endl;
    
    // 测试文件扩展名提取
    std::string ext = FileUtils::getFileExtension("test.epub");
    assert(ext == ".epub" || ext == "epub");
    std::cout << "File extension: " << ext << std::endl;
    
    // 测试路径连接
    std::string joined = FileUtils::joinPaths("dir1", "dir2", "file.txt");
    std::cout << "Joined path: " << joined << std::endl;
    
    std::cout << "FileUtils basic tests passed!" << std::endl;
}

void test_logger_basic() {
    std::cout << "Testing Logger basic functions..." << std::endl;
    
    // 测试日志级别设置
    Logger::setLevel(Logger::Level::INFO);
    std::cout << "Logger level set to INFO" << std::endl;
    
    // 测试日志输出（这些应该被输出）
    Logger::info("This is an info message");
    Logger::debug("This debug message should not appear if level is INFO");
    
    // 改变日志级别
    Logger::setLevel(Logger::Level::DEBUG);
    Logger::debug("Now this debug message should appear");
    
    std::cout << "Logger basic tests passed!" << std::endl;
}

int main() {
    std::cout << "Starting basic tests for EPUB Ad Cleaner..." << std::endl;
    
    try {
        test_file_utils_basic();
        test_logger_basic();
        
        std::cout << "\nAll basic tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}