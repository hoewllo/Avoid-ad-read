#ifndef AD_PATTERNS_H
#define AD_PATTERNS_H

#include <string>
#include <vector>
#include <regex>

namespace AdPatterns {
    // 广告模式类型
    enum class PatternType {
        GITHUB_PROJECT,      // GitHub项目推广
        UNICODE_SPECIAL,     // 特殊Unicode字符
        GENERAL_AD,          // 通用广告
        CUSTOM               // 自定义模式
    };
    
    // 广告模式结构
    struct AdPattern {
        PatternType type;
        std::string name;
        std::string pattern;
        std::string description;
        bool enabled;
    };
    
    // 获取所有内置广告模式
    std::vector<AdPattern> getBuiltinPatterns();
    
    // 从文件加载广告模式
    std::vector<std::regex> loadPatternsFromFile(const std::string& filePath);
    
    // 从字符串列表创建正则表达式
    std::vector<std::regex> createPatterns(const std::vector<std::string>& patternStrings);
    
    // 获取默认广告模式（作为正则表达式）
    std::vector<std::regex> getDefaultPatterns();
    
    // 验证正则表达式模式
    bool validatePattern(const std::string& pattern);
    
    // 转义正则表达式特殊字符
    std::string escapeRegex(const std::string& input);
    
    // 广告模式匹配器类
    class PatternMatcher {
    public:
        PatternMatcher();
        PatternMatcher(const std::vector<std::regex>& patterns);
        
        // 添加模式
        void addPattern(const std::regex& pattern);
        void addPattern(const std::string& pattern);
        
        // 匹配并替换广告内容
        std::string cleanContent(const std::string& content);
        
        // 检查是否包含广告
        bool containsAds(const std::string& content);
        
        // 获取匹配统计
        struct MatchStats {
            int totalMatches = 0;
            std::vector<std::string> matchedPatterns;
        };
        
        MatchStats getMatchStats() const { return matchStats; }
        void resetMatchStats();
        
    private:
        std::vector<std::regex> patterns;
        MatchStats matchStats;
    };
}

#endif // AD_PATTERNS_H