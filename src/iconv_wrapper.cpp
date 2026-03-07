#include "iconv_wrapper.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <cerrno>

// 检查是否定义了HAVE_ICONV
#ifdef HAVE_ICONV
    #include <iconv.h>
#else
    // 如果没有iconv，使用简化实现
    #include <algorithm>
    #ifdef _WIN32
        #include <windows.h>
    #endif
#endif

using namespace std;

#ifdef HAVE_ICONV

// 使用iconv的实现
IconvWrapper::IconvWrapper(const string& tocode, const string& fromcode) 
    : cd_(nullptr) {
    reset(tocode, fromcode);
}

IconvWrapper::~IconvWrapper() {
    cleanup();
}

IconvWrapper::IconvWrapper(IconvWrapper&& other) noexcept 
    : cd_(other.cd_) {
    other.cd_ = nullptr;
}

IconvWrapper& IconvWrapper::operator=(IconvWrapper&& other) noexcept {
    if (this != &other) {
        cleanup();
        cd_ = other.cd_;
        other.cd_ = nullptr;
    }
    return *this;
}

void IconvWrapper::cleanup() {
    if (cd_ != nullptr) {
        iconv_close(static_cast<iconv_t>(cd_));
        cd_ = nullptr;
    }
}

void IconvWrapper::reset(const string& tocode, const string& fromcode) {
    cleanup();
    
    // 创建新的转换描述符
    iconv_t cd = iconv_open(tocode.c_str(), fromcode.c_str());
    if (cd == reinterpret_cast<iconv_t>(-1)) {
        cerr << "错误: 无法创建iconv转换描述符 (" 
             << fromcode << " -> " << tocode << "): " 
             << strerror(errno) << endl;
        cd_ = nullptr;
    } else {
        cd_ = cd;
    }
}

string IconvWrapper::convert(const string& input) {
    if (!isValid() || input.empty()) {
        return input;
    }
    
    // 分配输出缓冲区（通常输入大小的4倍足够）
    size_t inbytesleft = input.size();
    size_t outbytesleft = input.size() * 4;
    vector<char> outbuf(outbytesleft);
    
    // 设置输入指针
    char* inptr = const_cast<char*>(input.data());
    char* outptr = outbuf.data();
    
    // 执行转换
    size_t result = iconv(static_cast<iconv_t>(cd_), 
                         &inptr, &inbytesleft, 
                         &outptr, &outbytesleft);
    
    if (result == static_cast<size_t>(-1)) {
        cerr << "错误: 编码转换失败: " << strerror(errno) << endl;
        return input;
    }
    
    // 计算实际输出大小
    size_t outputSize = outbuf.size() - outbytesleft;
    return string(outbuf.data(), outputSize);
}

#else

// 不使用iconv的简化实现
IconvWrapper::IconvWrapper(const string& tocode, const string& fromcode) 
    : cd_(nullptr) {
    // 简化实现中，cd_始终为nullptr
    (void)tocode; // 避免未使用参数警告
    (void)fromcode;
}

IconvWrapper::~IconvWrapper() {
    cleanup();
}

IconvWrapper::IconvWrapper(IconvWrapper&& other) noexcept 
    : cd_(other.cd_) {
    other.cd_ = nullptr;
}

IconvWrapper& IconvWrapper::operator=(IconvWrapper&& other) noexcept {
    if (this != &other) {
        cleanup();
        cd_ = other.cd_;
        other.cd_ = nullptr;
    }
    return *this;
}

void IconvWrapper::cleanup() {
    // 简化实现中无需清理
}

void IconvWrapper::reset(const string& tocode, const string& fromcode) {
    // 简化实现中不做任何操作
    (void)tocode;
    (void)fromcode;
}

string IconvWrapper::convert(const string& input) {
    // 简化实现：只处理常见的编码转换
    
    // 获取编码名称（转换为大写）
    string tocodeUpper = "UTF-8";  // 简化实现中固定目标编码为UTF-8
    string fromcodeUpper = "UTF-8"; // 简化实现中固定源编码为UTF-8
    
    // 在实际实现中，这里应该根据tocode和fromcode进行转换
    // 但为了简化，我们只返回输入字符串
    
#ifdef _WIN32
    // Windows平台的特殊处理
    // 这里可以添加Windows特有的编码转换逻辑
    // 例如：GBK <-> UTF-8
#endif
    
    return input;
}

#endif // HAVE_ICONV

// 便捷函数实现
string convertEncoding(const string& input, 
                      const string& toEncoding, 
                      const string& fromEncoding) {
    IconvWrapper converter(toEncoding, fromEncoding);
    if (converter.isValid()) {
        return converter.convert(input);
    }
    
    // 如果转换器无效，返回原始字符串
    cerr << "警告: 无法创建编码转换器 (" 
         << fromEncoding << " -> " << toEncoding << ")" << endl;
    return input;
}

string toUtf8(const string& input, const string& fromEncoding) {
    if (fromEncoding == "UTF-8" || fromEncoding == "utf-8") {
        return input;
    }
    return convertEncoding(input, "UTF-8", fromEncoding);
}

string fromUtf8(const string& input, const string& toEncoding) {
    if (toEncoding == "UTF-8" || toEncoding == "utf-8") {
        return input;
    }
    return convertEncoding(input, toEncoding, "UTF-8");
}