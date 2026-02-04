#ifndef EPUB_PROCESSOR_H
#define EPUB_PROCESSOR_H

#include <string>
#include <vector>
#include <regex>
#include <filesystem>

namespace fs = std::filesystem;

class EpubProcessor {
public:
    // 构造函数
    EpubProcessor(bool verbose = false, bool createBackup = true);
    
    // 处理单个EPUB文件
    bool processFile(const fs::path& inputPath, const fs::path& outputPath);
    
    // 批量处理目录
    bool processDirectory(const fs::path& inputDir, const fs::path& outputDir);
    
    // 设置广告模式
    void setAdPatterns(const std::vector<std::regex>& patterns);
    
    // 添加自定义广告模式
    void addAdPattern(const std::string& pattern);
    
    // 获取统计信息
    struct Stats {
        int filesProcessed = 0;
        int adsRemoved = 0;
        int errors = 0;
        std::vector<std::string> processedFiles;
    };
    
    const Stats& getStats() const { return stats; }
    
    // 重置统计信息
    void resetStats();
    
private:
    // 解压EPUB文件
    bool extractEpub(const fs::path& epubPath, const fs::path& extractDir);
    
    // 清理解压后的文件
    bool cleanExtractedFiles(const fs::path& extractDir);
    
    // 重新打包为EPUB
    bool repackEpub(const fs::path& extractDir, const fs::path& epubPath);
    
    // 清理单个XHTML文件
    bool cleanXhtmlFile(const fs::path& filePath);
    
    // 应用所有广告模式
    std::string applyAdPatterns(const std::string& content);
    
    // 创建备份
    bool createBackup(const fs::path& filePath);
    
    // 成员变量
    std::vector<std::regex> adPatterns;
    Stats stats;
    bool verbose;
    bool createBackupFiles;
    
    // 内置广告模式
    void initializeDefaultPatterns();
};

#endif // EPUB_PROCESSOR_H