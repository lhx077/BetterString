#ifndef BETTER_STRING_CORE_WRAPPER_H
#define BETTER_STRING_CORE_WRAPPER_H

#include "BetterStringCore.h"
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <sstream>
#include <functional>
#include <stdexcept>
#include <locale>
#include <codecvt>

namespace better {

class String {
private:
    bs_string_t m_str;

public:
    // 默认构造函数
    String() {
        bs_init(&m_str);
    }

    // 从C风格字符串构造
    String(const char* str) {
        bs_init_from_cstr(&m_str, str);
    }

    // 从C++标准字符串构造
    String(const std::string& str) {
        bs_init_from_cstr(&m_str, str.c_str());
    }

    // 复制构造函数
    String(const String& other) {
        bs_init_copy(&m_str, &other.m_str);
    }

    // 移动构造函数
    String(String&& other) noexcept {
        // 直接移动内存，避免复制
        m_str = other.m_str;
        
        // 重置other为空字符串，但不释放内存
        if (!other.m_str.storage.small.is_small) {
            // 如果是大字符串，需要防止析构函数释放内存
            other.m_str.storage.small.is_small = 1;
            other.m_str.storage.small.size = 0;
            other.m_str.storage.small.buffer[0] = '\0';
        }
    }

    // 析构函数
    ~String() {
        bs_free(&m_str);
    }

    // 复制赋值运算符
    String& operator=(const String& other) {
        if (this != &other) {
            bs_free(&m_str);
            bs_init_copy(&m_str, &other.m_str);
        }
        return *this;
    }

    // 移动赋值运算符
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            bs_free(&m_str);
            
            // 直接移动内存，避免复制
            m_str = other.m_str;
            
            // 重置other为空字符串，但不释放内存
            if (!other.m_str.storage.small.is_small) {
                // 如果是大字符串，需要防止析构函数释放内存
                other.m_str.storage.small.is_small = 1;
                other.m_str.storage.small.size = 0;
                other.m_str.storage.small.buffer[0] = '\0';
            }
        }
        return *this;
    }

    // 获取字符串长度
    size_t length() const {
        return bs_length(&m_str);
    }

    // 获取字符串容量
    size_t capacity() const {
        return m_str.storage.small.is_small ? 
               BS_SSO_BUFFER_SIZE - 1 : 
               m_str.storage.large.capacity - 1;
    }

    // 获取C风格字符串
    const char* c_str() const {
        return bs_c_str(&m_str);
    }

    // 转换为std::string
    std::string to_std_string() const {
        return std::string(c_str());
    }

    // 字符串连接
    String operator+(const String& other) const {
        String result;
        bs_free(&result.m_str); // 清理默认初始化的内容
        bs_concat(&result.m_str, &m_str, &other.m_str);
        return result;
    }

    // 字符串追加
    String& operator+=(const String& other) {
        bs_append(&m_str, &other.m_str);
        return *this;
    }

    // 字符串比较
    bool operator==(const String& other) const {
        return bs_compare(&m_str, &other.m_str) == 0;
    }

    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    bool operator<(const String& other) const {
        return bs_compare(&m_str, &other.m_str) < 0;
    }

    bool operator<=(const String& other) const {
        return bs_compare(&m_str, &other.m_str) <= 0;
    }

    bool operator>(const String& other) const {
        return bs_compare(&m_str, &other.m_str) > 0;
    }

    bool operator>=(const String& other) const {
        return bs_compare(&m_str, &other.m_str) >= 0;
    }

    // 访问单个字符
    char operator[](size_t index) const {
        if (index >= length()) {
            throw std::out_of_range("String index out of range");
        }
        return bs_char_at(&m_str, index);
    }

    char& operator[](size_t index) {
        if (index >= length()) {
            throw std::out_of_range("String index out of range");
        }
        
        // 返回字符引用，需要小心处理
        if (m_str.storage.small.is_small) {
            return m_str.storage.small.buffer[index];
        } else {
            return m_str.storage.large.data[index];
        }
    }

    // 子字符串
    String substring(size_t start, size_t count = std::string::npos) const {
        String result;
        bs_free(&result.m_str); // 清理默认初始化的内容
        bs_substring(&result.m_str, &m_str, start, count == std::string::npos ? BS_NPOS : count);
        return result;
    }

    // 查找子字符串
    size_t indexOf(const String& substr, size_t start = 0) const {
        size_t result = bs_index_of(&m_str, &substr.m_str, start);
        return result == BS_NPOS ? std::string::npos : result;
    }

    // 查找最后一个子字符串
    size_t lastIndexOf(const String& substr) const {
        size_t len = length();
        size_t subLen = substr.length();
        
        if (len == 0 || subLen == 0 || subLen > len) {
            return std::string::npos;
        }
        
        const char* str = c_str();
        const char* subStr = substr.c_str();
        
        for (size_t i = len - subLen; i != std::string::npos; --i) {
            if (strncmp(str + i, subStr, subLen) == 0) {
                return i;
            }
        }
        
        return std::string::npos;
    }

    // 检查是否包含子字符串
    bool contains(const String& substr) const {
        return indexOf(substr) != std::string::npos;
    }

    // 检查是否以指定字符串开头
    bool startsWith(const String& prefix) const {
        return bs_starts_with(&m_str, &prefix.m_str) != 0;
    }

    // 检查是否以指定字符串结尾
    bool endsWith(const String& suffix) const {
        return bs_ends_with(&m_str, &suffix.m_str) != 0;
    }

    // 替换子字符串
    String replace(const String& oldStr, const String& newStr) const {
        if (oldStr.length() == 0 || !contains(oldStr)) {
            return *this;
        }
        
        std::string result = to_std_string();
        std::string oldStdStr = oldStr.to_std_string();
        std::string newStdStr = newStr.to_std_string();
        
        size_t pos = 0;
        while ((pos = result.find(oldStdStr, pos)) != std::string::npos) {
            result.replace(pos, oldStdStr.length(), newStdStr);
            pos += newStdStr.length();
        }
        
        return String(result);
    }

    // 转换为大写
    String toUpper() const {
        std::string result = to_std_string();
        std::transform(result.begin(), result.end(), result.begin(), 
                      [](unsigned char c) { return std::toupper(c); });
        return String(result);
    }

    // 转换为小写
    String toLower() const {
        std::string result = to_std_string();
        std::transform(result.begin(), result.end(), result.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        return String(result);
    }

    // 去除首尾空白字符
    String trim() const {
        const char* str = c_str();
        size_t len = length();
        
        size_t start = 0;
        while (start < len && std::isspace(static_cast<unsigned char>(str[start]))) {
            ++start;
        }
        
        if (start == len) {
            return String();
        }
        
        size_t end = len - 1;
        while (end > start && std::isspace(static_cast<unsigned char>(str[end]))) {
            --end;
        }
        
        return substring(start, end - start + 1);
    }

    // 分割字符串
    std::vector<String> split(const String& delimiter) const {
        std::vector<String> result;
        
        if (length() == 0) {
            return result;
        }
        
        if (delimiter.length() == 0) {
            // 如果分隔符为空，则每个字符作为一个元素
            for (size_t i = 0; i < length(); ++i) {
                result.push_back(String(std::string(1, (*this)[i])));
            }
            return result;
        }
        
        size_t start = 0;
        size_t end = 0;
        size_t delimLen = delimiter.length();
        
        while ((end = indexOf(delimiter, start)) != std::string::npos) {
            result.push_back(substring(start, end - start));
            start = end + delimLen;
        }
        
        result.push_back(substring(start));
        return result;
    }

    // 连接字符串数组
    static String join(const std::vector<String>& strings, const String& delimiter) {
        if (strings.empty()) {
            return String();
        }
        
        if (strings.size() == 1) {
            return strings[0];
        }
        
        // 计算总长度
        size_t totalLength = 0;
        for (const auto& str : strings) {
            totalLength += str.length();
        }
        totalLength += delimiter.length() * (strings.size() - 1);
        
        // 创建结果字符串
        std::string result;
        result.reserve(totalLength);
        
        for (size_t i = 0; i < strings.size(); ++i) {
            result += strings[i].to_std_string();
            if (i < strings.size() - 1) {
                result += delimiter.to_std_string();
            }
        }
        
        return String(result);
    }

    // 格式化字符串
    template<typename... Args>
    static String format(const char* format, Args... args) {
        int size = snprintf(nullptr, 0, format, args...) + 1; // 额外的空间用于空字符
        if (size <= 0) {
            return String();
        }
        
        char* buf = new char[size];
        snprintf(buf, size, format, args...);
        
        String result(buf);
        delete[] buf;
        
        return result;
    }

    // 正则表达式匹配
    bool matches(const String& pattern) const {
        try {
            std::regex re(pattern.c_str());
            return std::regex_match(c_str(), re);
        } catch (const std::regex_error&) {
            return false;
        }
    }

    // 正则表达式替换
    String regexReplace(const String& pattern, const String& replacement) const {
        try {
            std::regex re(pattern.c_str());
            std::string result = std::regex_replace(to_std_string(), re, replacement.c_str());
            return String(result);
        } catch (const std::regex_error&) {
            return *this;
        }
    }

    // 转换为整数
    int toInt() const {
        try {
            return std::stoi(to_std_string());
        } catch (const std::exception&) {
            throw std::invalid_argument("Cannot convert string to int");
        }
    }

    // 转换为浮点数
    double toDouble() const {
        try {
            return std::stod(to_std_string());
        } catch (const std::exception&) {
            throw std::invalid_argument("Cannot convert string to double");
        }
    }

    // 从整数创建字符串
    static String fromInt(int value) {
        return String(std::to_string(value));
    }

    // 从浮点数创建字符串
    static String fromDouble(double value) {
        return String(std::to_string(value));
    }

    // 重复字符串
    String repeat(size_t count) const {
        if (count == 0 || length() == 0) {
            return String();
        }
        
        if (count == 1) {
            return *this;
        }
        
        size_t len = length();
        size_t totalLen = len * count;
        
        std::string result;
        result.reserve(totalLen);
        
        for (size_t i = 0; i < count; ++i) {
            result.append(c_str(), len);
        }
        
        return String(result);
    }

    // 反转字符串
    String reverse() const {
        size_t len = length();
        if (len <= 1) {
            return *this;
        }
        
        std::string result(len, '\0');
        const char* src = c_str();
        
        for (size_t i = 0; i < len; ++i) {
            result[i] = src[len - 1 - i];
        }
        
        return String(result);
    }
};

} // namespace better

#endif // BETTER_STRING_NEW_H