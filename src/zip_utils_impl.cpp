#include "zip_utils.h"
#include "file_utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

using namespace std;

namespace ZipUtils {
    
    // 简单的ZIP文件处理实现
    // 注意：这是一个简化的实现，真正的ZIP处理需要更复杂的逻辑
    
    // 检查文件是否为ZIP文件（通过文件头）
    bool isValidZipFile(const fs::path& zipPath) {
        if (!FileUtils::fileExists(zipPath)) {
            return false;
        }
        
        // 检查文件扩展名
        string ext = FileUtils::getFileExtension(zipPath);
        if (ext != ".zip" && ext != ".epub") {
            return false;
        }
        
        // 读取并检查ZIP文件头
        ifstream file(zipPath, ios::binary);
        if (!file.is_open()) {
            return false;
        }
        
        char header[4];
        file.read(header, 4);
        file.close();
        
        // ZIP文件头: PK\x03\x04
        return (header[0] == 'P' && header[1] == 'K' && 
                header[2] == 0x03 && header[3] == 0x04);
    }
    
    // 处理EPUB文件：复制为临时ZIP文件
    bool prepareEpubForProcessing(const fs::path& epubPath, fs::path& tempZipPath) {
        // 创建临时ZIP文件路径
        tempZipPath = fs::temp_directory_path() / ("temp_" + epubPath.filename().string() + ".zip");
        
        // 复制EPUB文件为ZIP文件
        ifstream epubFile(epubPath, ios::binary);
        ofstream zipFile(tempZipPath, ios::binary);
        
        if (!epubFile.is_open() || !zipFile.is_open()) {
            return false;
        }
        
        zipFile << epubFile.rdbuf();
        epubFile.close();
        zipFile.close();
        
        return true;
    }
    
    // 使用系统命令解压（跨平台）
    ZipResult extractUsingSystemCommand(const fs::path& zipPath, const fs::path& extractDir) {
        string command;
        int result = 0;
        
#ifdef _WIN32
        // Windows: 使用PowerShell的Expand-Archive
        command = "powershell -Command \"Expand-Archive -Path '" + 
                 zipPath.string() + "' -DestinationPath '" + 
                 extractDir.string() + "' -Force\"";
        result = system(command.c_str());
#else
        // Unix: 使用unzip
        command = "unzip -q -o '" + zipPath.string() + "' -d '" + 
                 extractDir.string() + "' 2>&1";
        result = system(command.c_str());
#endif
        
        if (result == 0) {
            return {ZipStatus::SUCCESS, "解压成功", result};
        } else {
            return {ZipStatus::SYSTEM_COMMAND_FAILED, 
                    "解压命令失败: " + command, result};
        }
    }
    
    // 使用系统命令压缩（跨平台）
    ZipResult createUsingSystemCommand(const fs::path& sourceDir, const fs::path& zipPath) {
        string command;
        int result = 0;
        
#ifdef _WIN32
        command = "powershell -Command \"Compress-Archive -Path '" + 
                 sourceDir.string() + "/*' -DestinationPath '" + 
                 zipPath.string() + "' -Force\"";
        result = system(command.c_str());
#else
        command = "cd '" + sourceDir.string() + "' && zip -r -q '" + 
                 zipPath.string() + "' . 2>&1";
        result = system(command.c_str());
#endif
        
        if (result == 0) {
            return {ZipStatus::SUCCESS, "压缩成功", result};
        } else {
            return {ZipStatus::SYSTEM_COMMAND_FAILED, 
                    "压缩命令失败: " + command, result};
        }
    }
    
    // 主解压函数
    ZipResult extractZip(const fs::path& zipPath, const fs::path& extractDir) {
        // 验证输入参数
        if (!FileUtils::fileExists(zipPath)) {
            return {ZipStatus::FILE_NOT_FOUND, 
                    "ZIP文件不存在: " + zipPath.string(), -1};
        }
        
        if (!FileUtils::createDirectory(extractDir)) {
            return {ZipStatus::DIRECTORY_NOT_FOUND, 
                    "无法创建解压目录: " + extractDir.string(), -1};
        }
        
        // 检查文件扩展名
        string ext = FileUtils::getFileExtension(zipPath);
        bool isEpub = (ext == ".epub");
        
        fs::path actualZipPath = zipPath;
        fs::path tempZipPath;
        
        if (isEpub) {
            // 为EPUB文件创建临时ZIP副本
            if (!prepareEpubForProcessing(zipPath, tempZipPath)) {
                return {ZipStatus::UNKNOWN_ERROR, 
                        "无法为EPUB文件创建临时ZIP副本", -1};
            }
            actualZipPath = tempZipPath;
        }
        
        // 使用系统命令解压
        ZipResult result = extractUsingSystemCommand(actualZipPath, extractDir);
        
        // 清理临时文件
        if (isEpub && !tempZipPath.empty()) {
            FileUtils::removeFile(tempZipPath);
        }
        
        return result;
    }
    
    // 主压缩函数
    ZipResult createZip(const fs::path& sourceDir, const fs::path& zipPath) {
        // 验证输入参数
        if (!FileUtils::directoryExists(sourceDir)) {
            return {ZipStatus::DIRECTORY_NOT_FOUND, 
                    "源目录不存在: " + sourceDir.string(), -1};
        }
        
        // 确保输出目录存在
        fs::path parentDir = zipPath.parent_path();
        if (!parentDir.empty()) {
            FileUtils::createDirectory(parentDir);
        }
        
        // 检查输出文件扩展名
        string ext = FileUtils::getFileExtension(zipPath);
        bool isEpub = (ext == ".epub");
        
        fs::path actualZipPath = zipPath;
        fs::path tempZipPath;
        
        if (isEpub) {
            // 创建临时ZIP文件
            tempZipPath = fs::temp_directory_path() / ("temp_" + zipPath.filename().string() + ".zip");
            actualZipPath = tempZipPath;
        }
        
        // 使用系统命令创建ZIP
        ZipResult result = createUsingSystemCommand(sourceDir, actualZipPath);
        
        // 如果是EPUB文件，重命名ZIP为EPUB
        if (isEpub && result.success() && !tempZipPath.empty()) {
            try {
                fs::rename(tempZipPath, zipPath);
            } catch (const fs::filesystem_error& e) {
                // 如果重命名失败，尝试复制
                ifstream src(tempZipPath, ios::binary);
                ofstream dst(zipPath, ios::binary);
                if (src.is_open() && dst.is_open()) {
                    dst << src.rdbuf();
                    src.close();
                    dst.close();
                    FileUtils::removeFile(tempZipPath);
                } else {
                    // 复制失败
                    return {ZipStatus::UNKNOWN_ERROR, 
                            "无法将ZIP文件转换为EPUB文件", -1};
                }
            }
        }
        
        return result;
    }
    
    // 列出ZIP文件内容
    vector<string> listZipContents(const fs::path& zipPath) {
        vector<string> contents;
        
        if (!isValidZipFile(zipPath)) {
            return contents;
        }
        
        // 检查文件扩展名
        string ext = FileUtils::getFileExtension(zipPath);
        bool isEpub = (ext == ".epub");
        
        fs::path actualZipPath = zipPath;
        fs::path tempZipPath;
        
        if (isEpub) {
            // 为EPUB文件创建临时ZIP副本
            if (!prepareEpubForProcessing(zipPath, tempZipPath)) {
                return contents;
            }
            actualZipPath = tempZipPath;
        }
        
        // 使用系统命令列出ZIP内容
        string command;
        
#ifdef _WIN32
        // Windows: 使用PowerShell
        command = "powershell -Command \"$zip = [System.IO.Compression.ZipFile]::OpenRead('" + 
                 actualZipPath.string() + "'); $zip.Entries | Select-Object FullName; $zip.Dispose()\"";
        
        FILE* pipe = _popen(command.c_str(), "r");
        if (pipe) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), pipe)) {
                string line(buffer);
                if (!line.empty() && line.find("FullName") == string::npos) {
                    // 清理字符串
                    line.erase(line.find_last_not_of("\n\r") + 1);
                    if (!line.empty()) {
                        contents.push_back(line);
                    }
                }
            }
            _pclose(pipe);
        }
#else
        // Unix: 使用unzip -l
        command = "unzip -l '" + actualZipPath.string() + "' | tail -n +4 | head -n -2";
        
        FILE* pipe = popen(command.c_str(), "r");
        if (pipe) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), pipe)) {
                string line(buffer);
                if (!line.empty()) {
                    // 提取文件名
                    size_t lastSpace = line.find_last_of(' ');
                    if (lastSpace != string::npos) {
                        string filename = line.substr(lastSpace + 1);
                        filename.erase(filename.find_last_not_of("\n\r") + 1);
                        if (!filename.empty()) {
                            contents.push_back(filename);
                        }
                    }
                }
            }
            pclose(pipe);
        }
#endif
        
        // 清理临时文件
        if (isEpub && !tempZipPath.empty()) {
            FileUtils::removeFile(tempZipPath);
        }
        
        return contents;
    }
    
    // 获取ZIP文件信息
    ZipFileInfo getZipFileInfo(const fs::path& zipPath) {
        ZipFileInfo info = {};
        
        if (!isValidZipFile(zipPath)) {
            return info;
        }
        
        // 获取文件大小
        info.compressedSize = FileUtils::getFileSize(zipPath);
        
        // 统计文件数量
        auto contents = listZipContents(zipPath);
        info.fileCount = static_cast<uint32_t>(contents.size());
        
        return info;
    }
}