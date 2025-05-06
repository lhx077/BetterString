#ifndef STRING_UTILS_CORE_H
#define STRING_UTILS_CORE_H

#include "BetterString.h"
#include <vector>
#include <random>
#include <chrono>

namespace better {

class StringUtils {
public:
    // 生成指定长度的随机字符串
    static String random(size_t length, const String& charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") {
        if (length == 0 || charset.length() == 0) {
            return String();
        }
        
        // 使用当前时间作为随机数种子
        unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
        std::mt19937 generator(seed);
        std::uniform_int_distribution<size_t> distribution(0, charset.length() - 1);
        
        std::string result;
        result.reserve(length);
        
        for (size_t i = 0; i < length; ++i) {
            result += charset[distribution(generator)];
        }
        
        return String(result);
    }
    
    // 计算两个字符串之间的Levenshtein距离（编辑距离）
    static size_t levenshteinDistance(const String& s1, const String& s2) {
        size_t len1 = s1.length();
        size_t len2 = s2.length();
        
        std::vector<std::vector<size_t>> dp(len1 + 1, std::vector<size_t>(len2 + 1, 0));
        
        for (size_t i = 0; i <= len1; ++i) {
            dp[i][0] = i;
        }
        
        for (size_t j = 0; j <= len2; ++j) {
            dp[0][j] = j;
        }
        
        for (size_t i = 1; i <= len1; ++i) {
            for (size_t j = 1; j <= len2; ++j) {
                size_t cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
                dp[i][j] = std::min({
                    dp[i - 1][j] + 1,      // 删除
                    dp[i][j - 1] + 1,      // 插入
                    dp[i - 1][j - 1] + cost // 替换
                });
            }
        }
        
        return dp[len1][len2];
    }
    
    // 计算两个字符串的相似度（基于编辑距离）
    static double similarity(const String& s1, const String& s2) {
        size_t maxLen = std::max(s1.length(), s2.length());
        if (maxLen == 0) {
            return 1.0; // 两个空字符串视为完全相同
        }
        
        size_t distance = levenshteinDistance(s1, s2);
        return 1.0 - static_cast<double>(distance) / maxLen;
    }
    
    // 将字符串转换为驼峰命名法
    static String toCamelCase(const String& str, bool capitalizeFirstLetter = false) {
        if (str.length() == 0) {
            return String();
        }
        
        std::vector<String> words = str.split(" ");
        String result;
        
        for (size_t i = 0; i < words.size(); ++i) {
            if (words[i].length() == 0) {
                continue;
            }
            
            if (i == 0 && !capitalizeFirstLetter) {
                result += words[i].toLower();
            } else {
                String word = words[i].toLower();
                if (word.length() > 0) {
                    word[0] = std::toupper(static_cast<unsigned char>(word[0]));
                }
                result += word;
            }
        }
        
        return result;
    }
    
    // 将字符串转换为蛇形命名法
    static String toSnakeCase(const String& str) {
        if (str.length() == 0) {
            return String();
        }
        
        std::string result;
        result.reserve(str.length() * 2); // 预留足够空间
        
        const char* cstr = str.c_str();
        for (size_t i = 0; i < str.length(); ++i) {
            char c = cstr[i];
            
            if (std::isupper(static_cast<unsigned char>(c))) {
                if (i > 0) {
                    result += '_';
                }
                result += std::tolower(static_cast<unsigned char>(c));
            } else {
                result += c;
            }
        }
        
        return String(result);
    }
    
    // 将字符串转换为kebab命名法（短横线命名法）
    static String toKebabCase(const String& str) {
        if (str.length() == 0) {
            return String();
        }
        
        std::string result;
        result.reserve(str.length() * 2); // 预留足够空间
        
        const char* cstr = str.c_str();
        for (size_t i = 0; i < str.length(); ++i) {
            char c = cstr[i];
            
            if (std::isupper(static_cast<unsigned char>(c))) {
                if (i > 0) {
                    result += '-';
                }
                result += std::tolower(static_cast<unsigned char>(c));
            } else {
                result += c;
            }
        }
        
        return String(result);
    }
    
    // 检查字符串是否为回文
    static bool isPalindrome(const String& str) {
        size_t len = str.length();
        if (len <= 1) {
            return true;
        }
        
        const char* cstr = str.c_str();
        for (size_t i = 0; i < len / 2; ++i) {
            if (cstr[i] != cstr[len - 1 - i]) {
                return false;
            }
        }
        
        return true;
    }
    
    // 计算字符串中的单词数量
    static size_t wordCount(const String& str) {
        if (str.length() == 0) {
            return 0;
        }
        
        std::vector<String> words = str.split(" ");
        size_t count = 0;
        
        for (const auto& word : words) {
            if (word.length() > 0) {
                ++count;
            }
        }
        
        return count;
    }
    
    // 截断字符串到指定长度，并添加省略号
    static String truncate(const String& str, size_t maxLength, const String& ellipsis = "...") {
        if (str.length() <= maxLength) {
            return str;
        }
        
        size_t truncateLength = maxLength - ellipsis.length();
        if (truncateLength < 0) {
            truncateLength = 0;
        }
        
        return str.substring(0, truncateLength) + ellipsis;
    }
};

} // namespace better

#endif // STRING_UTILS_CORE_H