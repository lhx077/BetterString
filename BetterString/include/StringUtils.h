#ifndef STRING_UTILS_H
#define STRING_UTILS_H

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
        
        for (size_t i = 0; i < str.length(); ++i) {
            char c = str[i];
            
            if (std::isspace(static_cast<unsigned char>(c))) {
                if (!result.empty() && result.back() != '_') {
                    result += '_';
                }
            } else if (std::isupper(static_cast<unsigned char>(c))) {
                if (i > 0 && !std::isspace(static_cast<unsigned char>(str[i - 1])) && 
                    result.back() != '_' && 
                    (i == str.length() - 1 || !std::isupper(static_cast<unsigned char>(str[i + 1])))) {
                    result += '_';
                }
                result += std::tolower(static_cast<unsigned char>(c));
            } else {
                result += c;
            }
        }
        
        return String(result);
    }
    
    // 将字符串转换为kebab-case命名法
    static String toKebabCase(const String& str) {
        String snakeCase = toSnakeCase(str);
        return snakeCase.replace("_", "-");
    }
    
    // 截断字符串到指定长度，并添加省略号
    static String truncate(const String& str, size_t maxLength, const String& ellipsis = "...") {
        if (str.length() <= maxLength) {
            return str;
        }
        
        size_t truncateLength = maxLength - ellipsis.length();
        if (truncateLength <= 0) {
            return ellipsis.substring(0, maxLength);
        }
        
        return str.substring(0, truncateLength) + ellipsis;
    }
    
    // 将字符串填充到指定长度
    static String pad(const String& str, size_t length, char padChar = ' ', bool padRight = true) {
        if (str.length() >= length) {
            return str;
        }
        
        size_t padLength = length - str.length();
        String padding(std::string(padLength, padChar));
        
        return padRight ? str + padding : padding + str;
    }
    
    // 计算字符串中指定子字符串的出现次数
    static size_t count(const String& str, const String& substr) {
        if (str.length() == 0 || substr.length() == 0 || substr.length() > str.length()) {
            return 0;
        }
        
        size_t count = 0;
        size_t pos = 0;
        
        while ((pos = str.indexOf(substr, pos)) != std::string::npos) {
            ++count;
            pos += substr.length();
        }
        
        return count;
    }
    
    // 检查字符串是否为回文
    static bool isPalindrome(const String& str) {
        size_t len = str.length();
        if (len <= 1) {
            return true;
        }
        
        for (size_t i = 0; i < len / 2; ++i) {
            if (str[i] != str[len - 1 - i]) {
                return false;
            }
        }
        
        return true;
    }
    
    // 将字符串中的HTML特殊字符转义
    static String escapeHtml(const String& str) {
        std::string result;
        result.reserve(str.length() * 2); // 预留足够空间
        
        for (size_t i = 0; i < str.length(); ++i) {
            char c = str[i];
            switch (c) {
                case '&': result += "&amp;"; break;
                case '<': result += "&lt;"; break;
                case '>': result += "&gt;"; break;
                case '"': result += "&quot;"; break;
                case '\'': result += "&#39;"; break;
                default: result += c; break;
            }
        }
        
        return String(result);
    }
    
    // 将转义的HTML字符还原
    static String unescapeHtml(const String& str) {
        String result = str;
        result = result.replace("&amp;", "&");
        result = result.replace("&lt;", "<");
        result = result.replace("&gt;", ">");
        result = result.replace("&quot;", "\"");
        result = result.replace("&#39;", "'");
        return result;
    }
};

} // namespace better

#endif // STRING_UTILS_H