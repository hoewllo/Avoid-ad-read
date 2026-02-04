#include "zip_utils.h"
#include "file_utils.h"
#include <zlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

namespace ZipUtils {
    
    // 使用zlib进行简单的数据压缩/解压缩
    // 注意：这些函数处理原始数据，不处理ZIP文件格式
    
    vector<unsigned char> compressData(const vector<unsigned char>& data) {
        vector<unsigned char> compressed;
        
        if (data.empty()) {
            return compressed;
        }
        
        // 计算压缩后的大小上限
        uLongf compressed_size = compressBound(data.size());
        compressed.resize(compressed_size);
        
        // 压缩数据
        if (compress(compressed.data(), &compressed_size, 
                     data.data(), data.size()) == Z_OK) {
            compressed.resize(compressed_size);
        } else {
            compressed.clear();
        }
        
        return compressed;
    }
    
    vector<unsigned char> decompressData(const vector<unsigned char>& compressed, 
                                        size_t original_size) {
        vector<unsigned char> decompressed(original_size);
        
        if (compressed.empty() || original_size == 0) {
            return decompressed;
        }
        
        uLongf dest_len = original_size;
        
        // 解压数据
        if (uncompress(decompressed.data(), &dest_len, 
                       compressed.data(), compressed.size()) == Z_OK) {
            decompressed.resize(dest_len);
        } else {
            decompressed.clear();
        }
        
        return decompressed;
    }
    
    // 压缩字符串
    string compressString(const string& str) {
        vector<unsigned char> data(str.begin(), str.end());
        vector<unsigned char> compressed = compressData(data);
        
        if (compressed.empty()) {
            return "";
        }
        
        return string(compressed.begin(), compressed.end());
    }
    
    // 解压字符串
    string decompressString(const string& compressed_str, size_t original_size) {
        vector<unsigned char> compressed(compressed_str.begin(), compressed_str.end());
        vector<unsigned char> decompressed = decompressData(compressed, original_size);
        
        if (decompressed.empty()) {
            return "";
        }
        
        return string(decompressed.begin(), decompressed.end());
    }
    
    // 计算数据的CRC32校验和
    uint32_t calculateCRC32(const vector<unsigned char>& data) {
        if (data.empty()) {
            return 0;
        }
        
        return crc32(0L, data.data(), data.size());
    }
    
    // 计算文件的CRC32校验和
    uint32_t calculateFileCRC32(const fs::path& filePath) {
        if (!FileUtils::fileExists(filePath)) {
            return 0;
        }
        
        // 读取文件内容
        string content = FileUtils::readFileToString(filePath);
        if (content.empty()) {
            return 0;
        }
        
        vector<unsigned char> data(content.begin(), content.end());
        return calculateCRC32(data);
    }
    
    // 检查数据完整性
    bool verifyDataIntegrity(const vector<unsigned char>& original, 
                            const vector<unsigned char>& decompressed) {
        if (original.size() != decompressed.size()) {
            return false;
        }
        
        return memcmp(original.data(), decompressed.data(), original.size()) == 0;
    }
    
    // 获取zlib版本信息
    string getZlibVersion() {
        return zlibVersion();
    }
    
    // 测试zlib功能
    bool testZlibFunctionality() {
        // 测试数据
        string test_data = "This is a test string for zlib compression and decompression.";
        vector<unsigned char> original(test_data.begin(), test_data.end());
        
        // 压缩
        vector<unsigned char> compressed = compressData(original);
        if (compressed.empty()) {
            cerr << "zlib压缩测试失败" << endl;
            return false;
        }
        
        // 解压
        vector<unsigned char> decompressed = decompressData(compressed, original.size());
        if (decompressed.empty()) {
            cerr << "zlib解压测试失败" << endl;
            return false;
        }
        
        // 验证
        if (!verifyDataIntegrity(original, decompressed)) {
            cerr << "zlib数据完整性验证失败" << endl;
            return false;
        }
        
        // 计算CRC32
        uint32_t crc = calculateCRC32(original);
        if (crc == 0 && !original.empty()) {
            cerr << "zlib CRC32计算失败" << endl;
            return false;
        }
        
        cout << "zlib功能测试通过:" << endl;
        cout << "  原始大小: " << original.size() << " 字节" << endl;
        cout << "  压缩后大小: " << compressed.size() << " 字节" << endl;
        cout << "  压缩率: " << (100.0 * compressed.size() / original.size()) << "%" << endl;
        cout << "  CRC32: 0x" << hex << crc << dec << endl;
        cout << "  zlib版本: " << getZlibVersion() << endl;
        
        return true;
    }
}