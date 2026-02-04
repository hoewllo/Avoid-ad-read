#include "ad_patterns.h"
#include "file_utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

namespace AdPatterns {
    
    vector<AdPattern> getBuiltinPatterns() {
        return {
            {
                PatternType::GITHUB_PROJECT,
                "github_project_ad",
                R"([\s\r\n]*【[^】]*使用[^】]*项目[^】]*进行[^】]*下载[^】]*：[^】]*】[\s\r\n]*)",
                "GitHub项目推广广告，包含'使用'、'项目'、'进行'、'下载'等关键词",
                true
            },
            {
                PatternType::GITHUB_PROJECT,
                "github_link_ad",
                R"([\s\r\n]*【[^】]*https?://[^】]*github[^】]*】[\s\r\n]*)",
                "包含GitHub链接的广告",
                true
            },
            {
                PatternType::GITHUB_PROJECT,
                "specific_project_ad",
                R"([\s\r\n]*【[^】]*POf-L/Fanqie-novel-Downloader[^】]*】[\s\r\n]*)",
                "特定项目名称'POf-L/Fanqie-novel-Downloader'的广告",
                true
            },
            {
                PatternType::GENERAL_AD,
                "download_ad",
                R"([\s\r\n]*【[^】]*下载[^】]*】[\s\r\n]*)",
                "包含'下载'关键词的广告",
                true
            },
            {
                PatternType::UNICODE_SPECIAL,
                "unicode_special_chars",
                R"([‌᠎⁠﻿\u200b\u200c\u200d\u2060\ufeff]+)",
                "特殊Unicode字符（零宽字符等）",
                true
            },
            {
                PatternType::GENERAL_AD,
                "empty_brackets",
                R"([\s\r\n]*【[\s]*】[\s\r\n]*)",
                "空方括号（可能包含空白字符）",
                true
            },
            {
                PatternType::GENERAL_AD,
                "project_download_ad",
                R"([\s\r\n]*【[^】]*项目[^】]*下载[^】]*】[\s\r\n]*)",
                "同时包含'项目'和'下载'的广告",
                true
            }
        };
    }
    
    vector<regex> loadPatternsFromFile(const string& filePath) {
        vector<regex> patterns;
        
        if (!FileUtils::fileExists(filePath)) {
            cerr << "错误: 模式文件不存在: " << filePath << endl;
            return patterns;
        }
        
        try {
            ifstream file(filePath);
            if (!file.is_open()) {
                cerr << "错误: 无法打开模式文件: " << filePath << endl;
                return patterns;
            }
            
            string line;
            int lineNum = 0;
            while (getline(file, line)) {
                lineNum++;
                
                // 跳过空行和注释
                if (line.empty() || line[0] == '#') {
                    continue;
                }
                
                // 去除首尾空白
                line.erase(0, line.find_first_not_of(" \t\r\n"));
                line.erase(line.find_last_not_of(" \t\r\n") + 1);
                
                if (line.empty()) {
                    continue;
                }
                
                // 验证并添加模式
                if (validatePattern(line)) {
                    try {
                        patterns.emplace_back(line, regex::optimize | regex::icase);
                    } catch (const regex_error& e) {
                        cerr << "警告: 第 " << lineNum << " 行无效的正则表达式: " << line << endl;
                        cerr << "错误信息: " << e.what() << endl;
                    }
                } else {
                    cerr << "警告: 第 " << lineNum << " 行无效的模式: " << line << endl;
                }
            }
            
            file.close();
            
            cout << "从文件加载了 " << patterns.size() << " 个广告模式" << endl;
            
        } catch (const exception& e) {
            cerr << "读取模式文件时发生异常: " << e.what() << endl;
        }
        
        return patterns;
    }
    
    vector<regex> createPatterns(const vector<string>& patternStrings) {
        vector<regex> patterns;
        patterns.reserve(patternStrings.size());
        
        for (const auto& patternStr : patternStrings) {
            try {
                patterns.emplace_back(patternStr, regex::optimize | regex::icase);
            } catch (const regex_error& e) {
                cerr << "警告: 无效的正则表达式模式: " << patternStr << endl;
                cerr << "错误信息: " << e.what() << endl;
            }
        }
        
        return patterns;
    }
    
    vector<regex> getDefaultPatterns() {
        auto builtinPatterns = getBuiltinPatterns();
        vector<string> patternStrings;
        
        for (const auto& pattern : builtinPatterns) {
            if (pattern.enabled) {
                patternStrings.push_back(pattern.pattern);
            }
        }
        
        return createPatterns(patternStrings);
    }
    
    bool validatePattern(const string& pattern) {
        if (pattern.empty()) {
            return false;
        }
        
        try {
            regex test(pattern, regex::optimize);
            return true;
        } catch (const regex_error&) {
            return false;
        }
    }
    
    string escapeRegex(const string& input) {
        static const string specialChars = "\\^$.|?*+()[]{}";
        string result;
        result.reserve(input.size() * 2);
        
        for (char c : input) {
            if (specialChars.find(c) != string::npos) {
                result += '\\';
            }
            result += c;
        }
        
        return result;
    }
    
    // PatternMatcher 实现
    PatternMatcher::PatternMatcher() {
        resetMatchStats();
    }
    
    PatternMatcher::PatternMatcher(const vector<regex>& patterns) 
        : patterns(patterns) {
        resetMatchStats();
    }
    
    void PatternMatcher::addPattern(const regex& pattern) {
        patterns.push_back(pattern);
    }
    
    void PatternMatcher::addPattern(const string& pattern) {
        try {
            patterns.emplace_back(pattern, regex::optimize | regex::icase);
        } catch (const regex_error& e) {
            cerr << "错误: 无效的正则表达式模式: " << pattern << endl;
            cerr << "错误信息: " << e.what() << endl;
        }
    }
    
    string PatternMatcher::cleanContent(const string& content) {
        string result = content;
        
        for (const auto& pattern : patterns) {
            try {
                string previous = result;
                result = regex_replace(result, pattern, "");
                
                // 统计匹配
                if (result != previous) {
                    matchStats.totalMatches++;
                    // 可以在这里记录匹配的详细信息
                }
                
            } catch (const regex_error& e) {
                cerr << "正则表达式错误: " << e.what() << endl;
            }
        }
        
        return result;
    }
    
    bool PatternMatcher::containsAds(const string& content) {
        for (const auto& pattern : patterns) {
            try {
                if (regex_search(content, pattern)) {
                    return true;
                }
            } catch (const regex_error& e) {
                cerr << "正则表达式错误: " << e.what() << endl;
            }
        }
        
        return false;
    }
    
    void PatternMatcher::resetMatchStats() {
        matchStats = MatchStats{};
    }
}