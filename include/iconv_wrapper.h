#ifndef ICONV_WRAPPER_H
#define ICONV_WRAPPER_H

#include <string>
#include <stdexcept>

class IconvWrapper {
public:
    // 构造函数，指定源编码和目标编码
    IconvWrapper(const std::string& tocode, const std::string& fromcode);
    
    // 析构函数
    ~IconvWrapper();
    
    // 禁用拷贝构造函数和赋值运算符
    IconvWrapper(const IconvWrapper&) = delete;
    IconvWrapper& operator=(const IconvWrapper&) = delete;
    
    // 允许移动语义
    IconvWrapper(IconvWrapper&& other) noexcept;
    IconvWrapper& operator=(IconvWrapper&& other) noexcept;
    
    // 转换字符串
    std::string convert(const std::string& input);
    
    // 检查是否有效
    bool isValid() const { return cd_ != nullptr; }
    
    // 重置转换描述符
    void reset(const std::string& tocode, const std::string& fromcode);
    
private:
    void* cd_;  // iconv转换描述符
    
    // 清理资源
    void cleanup();
};

// 便捷函数：转换编码
std::string convertEncoding(const std::string& input, 
                           const std::string& toEncoding, 
                           const std::string& fromEncoding);

// 便捷函数：转换为UTF-8
std::string toUtf8(const std::string& input, const std::string& fromEncoding);

// 便捷函数：从UTF-8转换
std::string fromUtf8(const std::string& input, const std::string& toEncoding);

#endif // ICONV_WRAPPER_H