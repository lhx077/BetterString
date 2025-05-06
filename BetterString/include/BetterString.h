#ifndef BETTER_STRING_H
#define BETTER_STRING_H

#include <cstring>
#include <string>
#include <vector>
#include <memory>
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
    // 小字符串优化 (SSO) 的缓冲区大小
    static constexpr size_t SSO_BUFFER_SIZE = 16;
    
    // 数据存储结构
    union {
        // 小字符串直接存储
        struct {
            char buffer[SSO_BUFFER_SIZE];
            uint8_t size;
            bool is_small;
        } small;
        
        // 大字符串使用动态分配
        struct {
            char* data;
            size_t size;
            size_t capacity;
            bool is_small;
        } large;
    } storage;

    // 初始化小字符串
    void initSmall(const char* str, size_t len) {
        storage.small.is_small = true;
        storage.small.size = static_cast<uint8_t>(len);
        if (str && len > 0) {
            memcpy(storage.small.buffer, str, len);
        }
        storage.small.buffer[len] = '\0';
    }

    // 初始化大字符串
    void initLarge(const char* str, size_t len) {
        storage.large.is_small = false;
        storage.large.size = len;
        storage.large.capacity = len + 1;
        storage.large.data = new char[storage.large.capacity];
        if (str && len > 0) {
            memcpy(storage.large.data, str, len);
        }
        storage.large.data[len] = '\0';
    }

    // 释放大字符串的内存
    void freeLarge() {
        if (!storage.large.is_small) {
            delete[] storage.large.data;
        }
    }

    // 确保容量足够
    void ensureCapacity(size_t capacity) {
        if (isSmall()) {
            if (capacity < SSO_BUFFER_SIZE) {
                return; // 仍然可以使用小字符串优化
            }
            
            // 从小字符串转换为大字符串
            char temp[SSO_BUFFER_SIZE];
            size_t tempSize = storage.small.size;
            memcpy(temp, storage.small.buffer, tempSize + 1);
            
            storage.large.is_small = false;
            storage.large.size = tempSize;
            storage.large.capacity = capacity + 1;
            storage.large.data = new char[storage.large.capacity];
            memcpy(storage.large.data, temp, tempSize + 1);
        } else {
            if (capacity <= storage.large.capacity - 1) {
                return; // 当前容量已足够
            }
            
            // 扩展大字符串容量
            size_t newCapacity = std::max(capacity + 1, storage.large.capacity * 2);
            char* newData = new char[newCapacity];
            memcpy(newData, storage.large.data, storage.large.size + 1);
            delete[] storage.large.data;
            
            storage.large.data = newData;
            storage.large.capacity = newCapacity;
        }
    }

    // 检查是否为小字符串
    bool isSmall() const {
        return storage.small.is_small;
    }

public:
    // 默认构造函数
    String() {
        initSmall(nullptr, 0);
    }

    // 从C风格字符串构造
    String(const char* str) {
        if (!str) {
            initSmall(nullptr, 0);
            return;
        }
        
        size_t len = strlen(str);
        if (len < SSO_BUFFER_SIZE) {
            initSmall(str, len);
        } else {
            initLarge(str, len);
        }
    }

    // 从C++标准字符串构造
    String(const std::string& str) {
        size_t len = str.length();
        if (len < SSO_BUFFER_SIZE) {
            initSmall(str.c_str(), len);
        } else {
            initLarge(str.c_str(), len);
        }
    }

    // 复制构造函数
    String(const String& other) {
        if (other.isSmall()) {
            storage.small = other.storage.small;
        } else {
            initLarge(other.storage.large.data, other.storage.large.size);
        }
    }

    // 移动构造函数
    String(String&& other) noexcept {
        if (other.isSmall()) {
            storage.small = other.storage.small;
        } else {
            storage.large = other.storage.large;
            other.initSmall(nullptr, 0); // 防止析构函数释放内存
        }
    }

    // 析构函数
    ~String() {
        if (!isSmall()) {
            freeLarge();
        }
    }

    // 复制赋值运算符
    String& operator=(const String& other) {
        if (this != &other) {
            if (!isSmall()) {
                freeLarge();
            }
            
            if (other.isSmall()) {
                storage.small = other.storage.small;
            } else {
                initLarge(other.storage.large.data, other.storage.large.size);
            }
        }
        return *this;
    }

    // 移动赋值运算符
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            if (!isSmall()) {
                freeLarge();
            }
            
            if (other.isSmall()) {
                storage.small = other.storage.small;
            } else {
                storage.large = other.storage.large;
                other.initSmall(nullptr, 0); // 防止析构函数释放内存
            }
        }
        return *this;
    }

    // 获取字符串长度
    size_t length() const {
        return isSmall() ? storage.small.size : storage.large.size;
    }

    // 获取字符串容量
    size_t capacity() const {
        return isSmall() ? SSO_BUFFER_SIZE - 1 : storage.large.capacity - 1;
    }

    // 获取C风格字符串
    const char* c_str() const {
        return isSmall() ? storage.small.buffer : storage.large.data;
    }

    // 转换为std::string
    std::string to_std_string() const {
        return std::string(c_str());
    }

    // 字符串连接
    String operator+(const String& other) const {
        String result;
        size_t thisLen = length();
        size_t otherLen = other.length();
        size_t totalLen = thisLen + otherLen;
        
        if (totalLen < SSO_BUFFER_SIZE) {
            result.initSmall(nullptr, totalLen);
            memcpy(result.storage.small.buffer, c_str(), thisLen);
            memcpy(result.storage.small.buffer + thisLen, other.c_str(), otherLen);
            result.storage.small.buffer[totalLen] = '\0';
        } else {
            result.initLarge(nullptr, totalLen);
            memcpy(result.storage.large.data, c_str(), thisLen);
            memcpy(result.storage.large.data + thisLen, other.c_str(), otherLen);
            result.storage.large.data[totalLen] = '\0';
        }
        
        return result;
    }

    // 字符串追加
    String& operator+=(const String& other) {
        size_t thisLen = length();
        size_t otherLen = other.length();
        size_t totalLen = thisLen + otherLen;
        
        ensureCapacity(totalLen);
        
        if (isSmall()) {
            memcpy(storage.small.buffer + thisLen, other.c_str(), otherLen);
            storage.small.buffer[totalLen] = '\0';
            storage.small.size = static_cast<uint8_t>(totalLen);
        } else {
            memcpy(storage.large.data + thisLen, other.c_str(), otherLen);
            storage.large.data[totalLen] = '\0';
            storage.large.size = totalLen;
        }
        
        return *this;
    }

    // 字符串比较
    bool operator==(const String& other) const {
        if (length() != other.length()) {
            return false;
        }
        return strcmp(c_str(), other.c_str()) == 0;
    }

    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    bool operator<(const String& other) const {
        return strcmp(c_str(), other.c_str()) < 0;
    }

    bool operator<=(const String& other) const {
        return strcmp(c_str(), other.c_str()) <= 0;
    }

    bool operator>(const String& other) const {
        return strcmp(c_str(), other.c_str()) > 0;
    }

    bool operator>=(const String& other) const {
        return strcmp(c_str(), other.c_str()) >= 0;
    }

    // 访问单个字符
    char operator[](size_t index) const {
        if (index >= length()) {
            throw std::out_of_range("String index out of range");
        }
        return isSmall() ? storage.small.buffer[index] : storage.large.data[index];
    }

    char& operator[](size_t index) {
        if (index >= length()) {
            throw std::out_of_range("String index out of range");
        }
        return isSmall() ? storage.small.buffer[index] : storage.large.data[index];
    }

    // 子字符串
    String substring(size_t start, size_t count = std::string::npos) const {
        size_t len = length();
        if (start >= len) {
            return String();
        }
        
        count = (count == std::string::npos || start + count > len) ? (len - start) : count;
        
        String result;
        if (count < SSO_BUFFER_SIZE) {
            result.initSmall(nullptr, count);
            memcpy(result.storage.small.buffer, c_str() + start, count);
            result.storage.small.buffer[count] = '\0';
        } else {
            result.initLarge(nullptr, count);
            memcpy(result.storage.large.data, c_str() + start, count);
            result.storage.large.data[count] = '\0';
        }
        
        return result;
    }

    // 查找子字符串
    size_t indexOf(const String& substr, size_t start = 0) const {
        if (start >= length() || substr.length() == 0) {
            return std::string::npos;
        }
        
        const char* pos = strstr(c_str() + start, substr.c_str());
        if (!pos) {
            return std::string::npos;
        }
        
        return pos - c_str();
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
        size_t prefixLen = prefix.length();
        if (prefixLen > length()) {
            return false;
        }
        return strncmp(c_str(), prefix.c_str(), prefixLen) == 0;
    }

    // 检查是否以指定字符串结尾
    bool endsWith(const String& suffix) const {
        size_t suffixLen = suffix.length();
        size_t len = length();
        if (suffixLen > len) {
            return false;
        }
        return strncmp(c_str() + (len - suffixLen), suffix.c_str(), suffixLen) == 0;
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
        
        size_t totalLength = 0;
        for (const auto& str : strings) {
            totalLength += str.length();
        }
        totalLength += delimiter.length() * (strings.size() - 1);
        
        String result;
        if (totalLength < SSO_BUFFER_SIZE) {
            result.initSmall(nullptr, totalLength);
            
            size_t pos = 0;
            for (size_t i = 0; i < strings.size(); ++i) {
                const String& str = strings[i];
                memcpy(result.storage.small.buffer + pos, str.c_str(), str.length());
                pos += str.length();
                
                if (i < strings.size() - 1) {
                    memcpy(result.storage.small.buffer + pos, delimiter.c_str(), delimiter.length());
                    pos += delimiter.length();
                }
            }
            result.storage.small.buffer[totalLength] = '\0';
        } else {
            result.initLarge(nullptr, totalLength);
            
            size_t pos = 0;
            for (size_t i = 0; i < strings.size(); ++i) {
                const String& str = strings[i];
                memcpy(result.storage.large.data + pos, str.c_str(), str.length());
                pos += str.length();
                
                if (i < strings.size() - 1) {
                    memcpy(result.storage.large.data + pos, delimiter.c_str(), delimiter.length());
                    pos += delimiter.length();
                }
            }
            result.storage.large.data[totalLength] = '\0';
        }
        
        return result;
    }

    // 格式化字符串
    template<typename... Args>
    static String format(const char* format, Args... args) {
        int size = snprintf(nullptr, 0, format, args...) + 1; // 额外的空间用于空字符
        if (size <= 0) {
            return String();
        }
        
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format, args...);
        
        return String(buf.get());
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
        
        String result;
        if (totalLen < SSO_BUFFER_SIZE) {
            result.initSmall(nullptr, totalLen);
            char* buffer = result.storage.small.buffer;
            for (size_t i = 0; i < count; ++i) {
                memcpy(buffer + i * len, c_str(), len);
            }
            buffer[totalLen] = '\0';
        } else {
            result.initLarge(nullptr, totalLen);
            char* data = result.storage.large.data;
            for (size_t i = 0; i < count; ++i) {
                memcpy(data + i * len, c_str(), len);
            }
            data[totalLen] = '\0';
        }
        
        return result;
    }

    // 反转字符串
    String reverse() const {
        size_t len = length();
        if (len <= 1) {
            return *this;
        }
        
        String result;
        if (len < SSO_BUFFER_SIZE) {
            result.initSmall(nullptr, len);
            for (size_t i = 0; i < len; ++i) {
                result.storage.small.buffer[i] = (*this)[len - 1 - i];
            }
            result.storage.small.buffer[len] = '\0';
        } else {
            result.initLarge(nullptr, len);
            for (size_t i = 0; i < len; ++i) {
                result.storage.large.data[i] = (*this)[len - 1 - i];
            }
            result.storage.large.data[len] = '\0';
        }
        
        return result;
    }
};

} // namespace better

#endif // BETTER_STRING_H