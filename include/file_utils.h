#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace FileUtils {
    // 文件操作
    bool fileExists(const fs::path& path);
    bool directoryExists(const fs::path& path);
    bool createDirectory(const fs::path& path);
    bool removeFile(const fs::path& path);
    bool removeDirectory(const fs::path& path);
    bool copyFile(const fs::path& src, const fs::path& dst);
    bool moveFile(const fs::path& src, const fs::path& dst);
    
    // 文件读写
    std::string readFileToString(const fs::path& path);
    bool writeStringToFile(const fs::path& path, const std::string& content);
    bool appendToFile(const fs::path& path, const std::string& content);
    
    // 文件搜索
    std::vector<fs::path> findFiles(const fs::path& directory, 
                                   const std::string& extension = ".xhtml");
    std::vector<fs::path> findFilesRecursive(const fs::path& directory, 
                                            const std::string& extension = ".xhtml");
    
    // 临时文件/目录管理
    class TempDirectory {
    public:
        TempDirectory(const std::string& prefix = "epub_cleaner_");
        ~TempDirectory();
        
        const fs::path& getPath() const { return path; }
        bool isValid() const { return !path.empty() && fs::exists(path); }
        
        // 禁止拷贝
        TempDirectory(const TempDirectory&) = delete;
        TempDirectory& operator=(const TempDirectory&) = delete;
        
        // 允许移动
        TempDirectory(TempDirectory&& other) noexcept;
        TempDirectory& operator=(TempDirectory&& other) noexcept;
        
    private:
        fs::path path;
    };
    
    // ZIP压缩/解压（现在通过ZipUtils模块处理）
    bool extractZip(const fs::path& zipPath, const fs::path& extractDir);
    bool createZip(const fs::path& sourceDir, const fs::path& zipPath);
    
    // 文件信息
    uint64_t getFileSize(const fs::path& path);
    std::string getFileExtension(const fs::path& path);
    std::string getFileNameWithoutExtension(const fs::path& path);
    
    // 路径操作
    fs::path getAbsolutePath(const fs::path& path);
    fs::path getRelativePath(const fs::path& path, const fs::path& base);
    bool isSubPath(const fs::path& path, const fs::path& base);
    
    // 编码转换
    std::string toUtf8(const std::string& str, const std::string& fromEncoding = "GBK");
    std::string fromUtf8(const std::string& str, const std::string& toEncoding = "GBK");
    
    // 文件比较
    bool filesAreEqual(const fs::path& path1, const fs::path& path2);
    
    // 备份管理
    bool createBackup(const fs::path& filePath, const std::string& suffix = ".bak");
    bool restoreBackup(const fs::path& filePath, const std::string& suffix = ".bak");
    bool backupExists(const fs::path& filePath, const std::string& suffix = ".bak");
}

#endif // FILE_UTILS_H