#include "epub_processor.h"
#include "ad_patterns.h"
#include "file_utils.h"
#include "logger.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <algorithm>

using namespace std;

// 命令行参数结构
struct CommandLineArgs {
    string inputPath;
    string outputPath;
    string inputDir;
    string outputDir;
    string patternFile;
    bool verbose = false;
    bool noBackup = false;
    bool showHelp = false;
    bool version = false;
    bool debug = false;
    bool quiet = false;
};

// 显示帮助信息
void showHelp() {
    cout << "EPUB广告清理工具 v1.0.0" << endl;
    cout << "用法: epub_cleaner [选项]" << endl << endl;
    cout << "选项:";
    cout << "\n  输入/输出:";
    cout << "\n    -i, --input FILE        输入EPUB文件路径";
    cout << "\n    -o, --output FILE       输出EPUB文件路径";
    cout << "\n    -I, --input-dir DIR     输入目录（批量处理）";
    cout << "\n    -O, --output-dir DIR    输出目录（批量处理）";
    cout << "\n  \n  广告模式:";
    cout << "\n    -p, --patterns FILE     自定义广告模式文件";
    cout << "\n    --list-patterns        列出所有内置广告模式";
    cout << "\n  \n  日志和输出:";
    cout << "\n    -v, --verbose           启用详细输出";
    cout << "\n    -q, --quiet             静默模式，只显示错误";
    cout << "\n    -d, --debug             启用调试输出";
    cout << "\n  \n  其他选项:";
    cout << "\n    -n, --no-backup         不创建备份文件";
    cout << "\n    -h, --help              显示此帮助信息";
    cout << "\n    -V, --version           显示版本信息";
    cout << "\n\n示例:";
    cout << "\n  epub_cleaner -i book.epub -o clean_book.epub";
    cout << "\n  epub_cleaner -I ./books -O ./cleaned_books -v";
    cout << "\n  epub_cleaner -i book.epub -p my_patterns.txt -d";
    cout << "\n  epub_cleaner -q -i book.epub -o clean_book.epub" << endl;
}

// 显示版本信息
void showVersion() {
    cout << "EPUB广告清理工具 v1.0.0" << endl;
    cout << "使用C++17和系统ZIP命令" << endl;
    cout << "编译时间: " << __DATE__ << " " << __TIME__ << endl;
}

// 列出内置广告模式
void listBuiltinPatterns() {
    auto patterns = AdPatterns::getBuiltinPatterns();
    
    cout << "内置广告模式 (共" << patterns.size() << "个):" << endl;
    cout << "========================================" << endl;
    
    for (const auto& pattern : patterns) {
        cout << "\n名称: " << pattern.name << endl;
        cout << "类型: ";
        switch (pattern.type) {
            case AdPatterns::PatternType::GITHUB_PROJECT:
                cout << "GitHub项目推广"; break;
            case AdPatterns::PatternType::UNICODE_SPECIAL:
                cout << "特殊Unicode字符"; break;
            case AdPatterns::PatternType::GENERAL_AD:
                cout << "通用广告"; break;
            case AdPatterns::PatternType::CUSTOM:
                cout << "自定义"; break;
        }
        cout << endl;
        cout << "描述: " << pattern.description << endl;
        cout << "模式: " << pattern.pattern << endl;
        cout << "启用: " << (pattern.enabled ? "是" : "否") << endl;
    }
}

// 解析命令行参数
CommandLineArgs parseArguments(int argc, char* argv[]) {
    CommandLineArgs args;
    
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        
        if (arg == "-i" || arg == "--input") {
            if (i + 1 < argc) args.inputPath = argv[++i];
        }
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) args.outputPath = argv[++i];
        }
        else if (arg == "-I" || arg == "--input-dir") {
            if (i + 1 < argc) args.inputDir = argv[++i];
        }
        else if (arg == "-O" || arg == "--output-dir") {
            if (i + 1 < argc) args.outputDir = argv[++i];
        }
        else if (arg == "-p" || arg == "--patterns") {
            if (i + 1 < argc) args.patternFile = argv[++i];
        }
        else if (arg == "-v" || arg == "--verbose") {
            args.verbose = true;
        }
        else if (arg == "-d" || arg == "--debug") {
            args.debug = true;
        }
        else if (arg == "-q" || arg == "--quiet") {
            args.quiet = true;
        }
        else if (arg == "-n" || arg == "--no-backup") {
            args.noBackup = true;
        }
        else if (arg == "-h" || arg == "--help") {
            args.showHelp = true;
        }
        else if (arg == "-V" || arg == "--version") {
            args.version = true;
        }
        else if (arg == "--list-patterns") {
            listBuiltinPatterns();
            exit(0);
        }
        else if (arg[0] == '-') {
            cerr << "错误: 未知选项 " << arg << endl;
            args.showHelp = true;
        }
        else {
            // 如果没有指定输入选项，假设第一个非选项参数是输入文件
            if (args.inputPath.empty() && args.inputDir.empty()) {
                args.inputPath = arg;
            }
        }
    }
    
    return args;
}

// 验证参数
bool validateArguments(const CommandLineArgs& args) {
    if (args.showHelp || args.version) {
        return true;
    }
    
    if (args.inputPath.empty() && args.inputDir.empty()) {
        cerr << "错误: 必须指定输入文件或输入目录" << endl;
        return false;
    }
    
    if (!args.inputPath.empty() && !args.inputDir.empty()) {
        cerr << "错误: 不能同时指定输入文件和输入目录" << endl;
        return false;
    }
    
    if (!args.inputPath.empty()) {
        if (!FileUtils::fileExists(args.inputPath)) {
            cerr << "错误: 输入文件不存在: " << args.inputPath << endl;
            return false;
        }
        
        if (args.outputPath.empty()) {
            LOG_INFO << "未指定输出文件，将使用默认命名规则";
        }
    }
    
    if (!args.inputDir.empty()) {
        if (!FileUtils::directoryExists(args.inputDir)) {
            cerr << "错误: 输入目录不存在: " << args.inputDir << endl;
            return false;
        }
        
        if (args.outputDir.empty()) {
            LOG_INFO << "未指定输出目录，将使用默认命名规则";
        }
    }
    
    if (!args.patternFile.empty() && !FileUtils::fileExists(args.patternFile)) {
        cerr << "错误: 广告模式文件不存在: " << args.patternFile << endl;
        return false;
    }
    
    // 检查参数冲突
    if (args.quiet && (args.verbose || args.debug)) {
        cerr << "警告: --quiet 选项会覆盖 --verbose 和 --debug 选项" << endl;
    }
    
    return true;
}

// 生成默认输出路径
string getDefaultOutputPath(const string& inputPath) {
    namespace fs = std::filesystem;
    fs::path path(inputPath);
    string stem = path.stem().string();
    string ext = path.extension().string();
    return (path.parent_path() / (stem + "_clean" + ext)).string();
}

// 生成默认输出目录
string getDefaultOutputDir(const string& inputDir) {
    namespace fs = std::filesystem;
    fs::path dir(inputDir);
    return (dir.parent_path() / (dir.filename().string() + "_cleaned")).string();
}

int main(int argc, char* argv[]) {
    // 设置默认日志级别
    Logger::setLevel(Logger::Level::INFO);
    
    // 解析命令行参数
    CommandLineArgs args = parseArguments(argc, argv);
    
    // 显示帮助或版本信息
    if (args.showHelp) {
        showHelp();
        return 0;
    }
    
    if (args.version) {
        showVersion();
        return 0;
    }
    
    // 设置日志级别
    if (args.quiet) {
        Logger::setLevel(Logger::Level::ERROR);
    } else if (args.debug) {
        Logger::setLevel(Logger::Level::DEBUG);
    } else if (args.verbose) {
        Logger::setLevel(Logger::Level::INFO);
    }
    
    // 验证参数
    if (!validateArguments(args)) {
        showHelp();
        return 1;
    }
    
    try {
        LOG_INFO << "EPUB广告清理工具启动";
        
        // 创建EPUB处理器
        EpubProcessor processor(args.verbose, !args.noBackup);
        
        // 加载自定义广告模式（如果指定）
        if (!args.patternFile.empty()) {
            LOG_INFO << "加载自定义广告模式文件: " << args.patternFile;
            auto patterns = AdPatterns::loadPatternsFromFile(args.patternFile);
            processor.setAdPatterns(patterns);
            LOG_INFO << "已加载 " << patterns.size() << " 个自定义广告模式";
        }
        
        bool success = false;
        
        // 处理单个文件
        if (!args.inputPath.empty()) {
            string outputPath = args.outputPath;
            if (outputPath.empty()) {
                outputPath = getDefaultOutputPath(args.inputPath);
                LOG_INFO << "使用默认输出路径: " << outputPath;
            }
            
            LOG_INFO << "开始处理文件: " << args.inputPath;
            LOG_INFO << "输出到: " << outputPath;
            
            success = processor.processFile(args.inputPath, outputPath);
            
            if (success) {
                auto stats = processor.getStats();
                LOG_INFO << "\n处理完成!";
                LOG_INFO << "文件: " << args.inputPath << " -> " << outputPath;
                LOG_INFO << "移除广告: " << stats.adsRemoved << " 处";
                if (stats.errors > 0) {
                    LOG_WARN << "警告: 处理过程中遇到 " << stats.errors << " 个错误";
                }
            }
        }
        // 处理目录
        else if (!args.inputDir.empty()) {
            string outputDir = args.outputDir;
            if (outputDir.empty()) {
                outputDir = getDefaultOutputDir(args.inputDir);
                LOG_INFO << "使用默认输出目录: " << outputDir;
            }
            
            // 创建输出目录
            if (!FileUtils::createDirectory(outputDir)) {
                LOG_ERROR << "错误: 无法创建输出目录: " << outputDir;
                return 1;
            }
            
            LOG_INFO << "开始批量处理目录: " << args.inputDir;
            LOG_INFO << "输出到目录: " << outputDir;
            
            success = processor.processDirectory(args.inputDir, outputDir);
            
            if (success) {
                auto stats = processor.getStats();
                LOG_INFO << "\n批量处理完成!";
                LOG_INFO << "处理文件数: " << stats.filesProcessed;
                LOG_INFO << "移除广告总数: " << stats.adsRemoved << " 处";
                if (stats.errors > 0) {
                    LOG_WARN << "警告: 处理过程中遇到 " << stats.errors << " 个错误";
                }
            }
        }
        
        if (!success) {
            LOG_ERROR << "\n处理失败!";
            return 1;
        }
        
        LOG_INFO << "程序执行完成";
        return 0;
        
    } catch (const exception& e) {
        LOG_FATAL << "程序异常: " << e.what();
        return 1;
    }
}