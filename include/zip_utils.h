#ifndef ZIP_UTILS_H
#define ZIP_UTILS_H

#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace ZipUtils {
    // ZIP文件操作状态
    enum class ZipStatus {
        SUCCESS,
        FILE_NOT_FOUND,
        DIRECTORY_NOT_FOUND,
        PERMISSION_DENIED,
        SYSTEM_COMMAND_FAILED,
        UNKNOWN_ERROR
    };
    
    // ZIP操作结果
    struct ZipResult {
        ZipStatus status;
        std::string message;
        int exitCode;
        
        bool success() const { return status == ZipStatus::SUCCESS; }
        explicit operator bool() const { return success(); }
    };
    
    // 解压ZIP文件
    ZipResult extractZip(const fs::path& zipPath, const fs::path& extractDir);
    
    // 创建ZIP文件
    ZipResult createZip(const fs::path& sourceDir, const fs::path& zipPath);
    
    // 列出ZIP文件内容
    std::vector<std::string> listZipContents(const fs::path& zipPath);
    
    // 检查文件是否为有效的ZIP文件
    bool isValidZipFile(const fs::path& zipPath);
    
    // 获取ZIP文件信息
    struct ZipFileInfo {
        uint64_t compressedSize;
        uint64_t uncompressedSize;
        uint32_t fileCount;
        std::string comment;
    };
    
    ZipFileInfo getZipFileInfo(const fs::path& zipPath);
    
    // 平台特定的ZIP实现
    namespace Platform {
        // 后备解压实现
        ZipResult extractZipFallback(const fs::path& zipPath, const fs::path& extractDir);
        
        // 后备压缩实现
        ZipResult createZipFallback(const fs::path& sourceDir, const fs::path& zipPath);
        
#ifdef _WIN32
        // Windows特定实现
        ZipResult extractZipWindows(const fs::path& zipPath, const fs::path& extractDir);
        ZipResult createZipWindows(const fs::path& sourceDir, const fs::path& zipPath);
#else
        // Unix特定实现
        ZipResult extractZipUnix(const fs::path& zipPath, const fs::path& extractDir);
        ZipResult createZipUnix(const fs::path& sourceDir, const fs::path& zipPath);
#endif
    }
}

#endif // ZIP_UTILS_H