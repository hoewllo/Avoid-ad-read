#include "file_utils.h"
#include "zip_utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <random>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
    #include <shlwapi.h>
    #include <direct.h>
    #define getcwd _getcwd
    #define chdir _chdir
    #ifdef _MSC_VER
        #pragma comment(lib, "shlwapi.lib")
    #endif
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #include <limits.h>
#endif

using namespace std;

namespace FileUtils {
    
    // ==================== 文件操作 ====================
    
    bool fileExists(const fs::path& path) {
        try {
            return fs::exists(path) && fs::is_regular_file(path);
        } catch (const fs::filesystem_error&) {
            return false;
        }
    }
    
    bool directoryExists(const fs::path& path) {
        try {
            return fs::exists(path) && fs::is_directory(path);
        } catch (const fs::filesystem_error&) {
            return false;
        }
    }
    
    bool createDirectory(const fs::path& path) {
        try {
            if (directoryExists(path)) {
                return true;
            }
            return fs::create_directories(path);
        } catch (const fs::filesystem_error& e) {
            cerr << "创建目录失败: " << path << " - " << e.what() << endl;
            return false;
        }
    }
    
    bool removeFile(const fs::path& path) {
        try {
            if (!fileExists(path)) {
                return true;
            }
            return fs::remove(path);
        } catch (const fs::filesystem_error& e) {
            cerr << "删除文件失败: " << path << " - " << e.what() << endl;
            return false;
        }
    }
    
    bool removeDirectory(const fs::path& path) {
        try {
            if (!directoryExists(path)) {
                return true;
            }
            return fs::remove_all(path) > 0;
        } catch (const fs::filesystem_error& e) {
            cerr << "删除目录失败: " << path << " - " << e.what() << endl;
            return false;
        }
    }
    
    bool copyFile(const fs::path& src, const fs::path& dst) {
        try {
            if (!fileExists(src)) {
                cerr << "源文件不存在: " << src << endl;
                return false;
            }
            
            // 确保目标目录存在
            createDirectory(dst.parent_path());
            
            fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
            return true;
        } catch (const fs::filesystem_error& e) {
            cerr << "复制文件失败: " << src << " -> " << dst << " - " << e.what() << endl;
            return false;
        }
    }
    
    bool moveFile(const fs::path& src, const fs::path& dst) {
        try {
            if (!fileExists(src)) {
                cerr << "源文件不存在: " << src << endl;
                return false;
            }
            
            // 确保目标目录存在
            createDirectory(dst.parent_path());
            
            fs::rename(src, dst);
            return true;
        } catch (const fs::filesystem_error& e) {
            // 如果跨设备移动失败，尝试复制后删除
            if (copyFile(src, dst)) {
                return removeFile(src);
            }
            cerr << "移动文件失败: " << src << " -> " << dst << " - " << e.what() << endl;
            return false;
        }
    }
    
    // ==================== 文件读写 ====================
    
    string readFileToString(const fs::path& path) {
        if (!fileExists(path)) {
            cerr << "文件不存在: " << path << endl;
            return "";
        }
        
        try {
            ifstream file(path, ios::binary | ios::ate);
            if (!file.is_open()) {
                cerr << "无法打开文件: " << path << endl;
                return "";
            }
            
            streamsize size = file.tellg();
            file.seekg(0, ios::beg);
            
            string content;
            content.resize(static_cast<size_t>(size));
            
            if (!file.read(&content[0], size)) {
                cerr << "读取文件失败: " << path << endl;
                return "";
            }
            
            return content;
        } catch (const exception& e) {
            cerr << "读取文件时发生异常: " << path << " - " << e.what() << endl;
            return "";
        }
    }
    
    bool writeStringToFile(const fs::path& path, const string& content) {
        try {
            // 确保目录存在
            createDirectory(path.parent_path());
            
            ofstream file(path, ios::binary);
            if (!file.is_open()) {
                cerr << "无法创建文件: " << path << endl;
                return false;
            }
            
            file.write(content.data(), static_cast<streamsize>(content.size()));
            return file.good();
        } catch (const exception& e) {
            cerr << "写入文件时发生异常: " << path << " - " << e.what() << endl;
            return false;
        }
    }
    
    bool appendToFile(const fs::path& path, const string& content) {
        try {
            // 确保目录存在
            createDirectory(path.parent_path());
            
            ofstream file(path, ios::binary | ios::app);
            if (!file.is_open()) {
                cerr << "无法打开文件: " << path << endl;
                return false;
            }
            
            file.write(content.data(), static_cast<streamsize>(content.size()));
            return file.good();
        } catch (const exception& e) {
            cerr << "追加文件时发生异常: " << path << " - " << e.what() << endl;
            return false;
        }
    }
    
    // ==================== 文件搜索 ====================
    
    vector<fs::path> findFiles(const fs::path& directory, const string& extension) {
        vector<fs::path> files;
        
        if (!directoryExists(directory)) {
            cerr << "目录不存在: " << directory << endl;
            return files;
        }
        
        try {
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (entry.is_regular_file() && 
                    (extension.empty() || entry.path().extension() == extension)) {
                    files.push_back(entry.path());
                }
            }
        } catch (const fs::filesystem_error& e) {
            cerr << "搜索文件时发生异常: " << directory << " - " << e.what() << endl;
        }
        
        return files;
    }
    
    vector<fs::path> findFilesRecursive(const fs::path& directory, const string& extension) {
        vector<fs::path> files;
        
        if (!directoryExists(directory)) {
            cerr << "目录不存在: " << directory << endl;
            return files;
        }
        
        try {
            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file() && 
                    (extension.empty() || entry.path().extension() == extension)) {
                    files.push_back(entry.path());
                }
            }
        } catch (const fs::filesystem_error& e) {
            cerr << "递归搜索文件时发生异常: " << directory << " - " << e.what() << endl;
        }
        
        return files;
    }
    
    // ==================== 临时目录管理 ====================
    
    TempDirectory::TempDirectory(const string& prefix) {
        // 生成随机目录名
        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(
            now.time_since_epoch()).count();
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1000, 9999);
        
        string dirName = prefix + to_string(timestamp) + "_" + to_string(dis(gen));
        fs::path tempDir = fs::temp_directory_path() / dirName;
        
        if (createDirectory(tempDir)) {
            path = tempDir;
        }
    }
    
    TempDirectory::~TempDirectory() {
        if (!path.empty() && directoryExists(path)) {
            removeDirectory(path);
        }
    }
    
    TempDirectory::TempDirectory(TempDirectory&& other) noexcept 
        : path(move(other.path)) {
        other.path.clear();
    }
    
    TempDirectory& TempDirectory::operator=(TempDirectory&& other) noexcept {
        if (this != &other) {
            // 清理当前目录
            if (!path.empty() && directoryExists(path)) {
                removeDirectory(path);
            }
            path = move(other.path);
            other.path.clear();
        }
        return *this;
    }
    
    // ==================== ZIP压缩/解压 ====================
    
    bool extractZip(const fs::path& zipPath, const fs::path& extractDir) {
        auto result = ZipUtils::extractZip(zipPath, extractDir);
        if (!result.success()) {
            cerr << "解压ZIP失败: " << result.message << endl;
        }
        return result.success();
    }
    
    bool createZip(const fs::path& sourceDir, const fs::path& zipPath) {
        auto result = ZipUtils::createZip(sourceDir, zipPath);
        if (!result.success()) {
            cerr << "创建ZIP失败: " << result.message << endl;
        }
        return result.success();
    }
    
    // ==================== 文件信息 ====================
    
    uint64_t getFileSize(const fs::path& path) {
        try {
            if (fileExists(path)) {
                return static_cast<uint64_t>(fs::file_size(path));
            }
        } catch (const fs::filesystem_error&) {
            // 忽略错误
        }
        return 0;
    }
    
    string getFileExtension(const fs::path& path) {
        return path.extension().string();
    }
    
    string getFileNameWithoutExtension(const fs::path& path) {
        return path.stem().string();
    }
    
    // ==================== 路径操作 ====================
    
    fs::path getAbsolutePath(const fs::path& path) {
        try {
            return fs::absolute(path);
        } catch (const fs::filesystem_error&) {
            return path;
        }
    }
    
    fs::path getRelativePath(const fs::path& path, const fs::path& base) {
        try {
            return fs::relative(path, base);
        } catch (const fs::filesystem_error&) {
            return path;
        }
    }
    
    bool isSubPath(const fs::path& path, const fs::path& base) {
        try {
            auto relative = fs::relative(path, base);
            return !relative.empty() && relative.string().find("..") == string::npos;
        } catch (const fs::filesystem_error&) {
            return false;
        }
    }
    
    // ==================== 编码转换 ====================
    
    string toUtf8(const string& str, const string& fromEncoding) {
        // 简化实现，实际项目中应该使用iconv或ICU库
        // 这里假设输入已经是UTF-8或ASCII
        return str;
    }
    
    string fromUtf8(const string& str, const string& toEncoding) {
        // 简化实现
        return str;
    }
    
    // ==================== 文件比较 ====================
    
    bool filesAreEqual(const fs::path& path1, const fs::path& path2) {
        if (!fileExists(path1) || !fileExists(path2)) {
            return false;
        }
        
        if (getFileSize(path1) != getFileSize(path2)) {
            return false;
        }
        
        // 比较文件内容
        string content1 = readFileToString(path1);
        string content2 = readFileToString(path2);
        
        return content1 == content2;
    }
    
    // ==================== 备份管理 ====================
    
    bool createBackup(const fs::path& filePath, const string& suffix) {
        if (!fileExists(filePath)) {
            cerr << "无法备份不存在的文件: " << filePath << endl;
            return false;
        }
        
        fs::path backupPath = filePath.string() + suffix;
        
        // 如果备份文件已存在，先删除
        if (fileExists(backupPath)) {
            removeFile(backupPath);
        }
        
        return copyFile(filePath, backupPath);
    }
    
    bool restoreBackup(const fs::path& filePath, const string& suffix) {
        fs::path backupPath = filePath.string() + suffix;
        
        if (!fileExists(backupPath)) {
            cerr << "备份文件不存在: " << backupPath << endl;
            return false;
        }
        
        return copyFile(backupPath, filePath);
    }
    
    bool backupExists(const fs::path& filePath, const string& suffix) {
        fs::path backupPath = filePath.string() + suffix;
        return fileExists(backupPath);
    }
}
