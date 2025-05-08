/**
 * better_string_single.hpp - 高性能字符串库的C++实现 (单文件版本)
 * 
 * 这个库提供了一个高性能、功能丰富的字符串处理库，
 * 支持Unicode和各种现代字符串操作。底层基于C实现以提高性能。
 */

#ifndef BETTER_STRING_SINGLE_HPP
#define BETTER_STRING_SINGLE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <locale>
#include <iostream>
#include <cstdarg>
#include <algorithm>
#include <sstream>
#include <codecvt>

// 首先包含C实现（需要在使用前定义BETTER_STRING_IMPLEMENTATION）
#ifndef BETTER_STRING_IMPLEMENTATION
#define BETTER_STRING_IMPLEMENTATION
#endif

// 现在包含C实现
#include "better_string_single.h"

namespace bs {

/**
 * 字符串类
 */
class String {
public:
    // 构造函数
    String() : m_str(bs_string_new("")) {}
    
    String(const char* str) : m_str(bs_string_new(str)) {}
    
    String(const std::string& str) : m_str(bs_string_new_len(str.c_str(), str.length())) {}
    
    String(const String& other) : m_str(bs_string_copy(other.m_str)) {}
    
    String(String&& other) noexcept : m_str(other.m_str) {
        other.m_str = nullptr;
    }
    
    explicit String(bs_string* str, bool own = true) : m_str(own ? str : bs_string_copy(str)) {}
    
    ~String() {
        freeString();
    }
    
    // 赋值操作符
    String& operator=(const String& other) {
        if (this != &other) {
            freeString();
            m_str = bs_string_copy(other.m_str);
        }
        return *this;
    }
    
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            freeString();
            m_str = other.m_str;
            other.m_str = nullptr;
        }
        return *this;
    }
    
    String& operator=(const char* str) {
        freeString();
        m_str = bs_string_new(str);
        return *this;
    }
    
    String& operator=(const std::string& str) {
        freeString();
        m_str = bs_string_new_len(str.c_str(), str.length());
        return *this;
    }
    
    // 基本属性
    size_t length() const {
        return bs_string_length(m_str);
    }
    
    size_t byteLength() const {
        return bs_string_byte_length(m_str);
    }
    
    bool isEmpty() const {
        return length() == 0;
    }
    
    /**
     * 计算字符串中的单词数量
     * @return 单词数量
     */
    size_t wordCount() const {
        return bs_string_word_count(m_str);
    }
    
    // 转换
    const char* c_str() const {
        return bs_string_c_str(m_str);
    }
    
    std::string toString() const {
        return std::string(c_str(), byteLength());
    }
    
    std::u16string toUtf16() const {
        // 简单实现，使用标准库转换UTF-8到UTF-16
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
        return converter.from_bytes(c_str());
    }
    
    std::u32string toUtf32() const {
        // 简单实现，使用标准库转换UTF-8到UTF-32
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        return converter.from_bytes(c_str());
    }
    
    // 操作
    String substring(size_t start, size_t length = std::string::npos) const {
        if (length == std::string::npos) {
            length = this->length() - start;
        }
        bs_string* sub = bs_string_substring(m_str, start, length);
        return String(sub);
    }
    
    String& append(const String& str) {
        m_str = bs_string_append(m_str, str.m_str);
        return *this;
    }
    
    String& append(const char* str) {
        m_str = bs_string_append_cstr(m_str, str);
        return *this;
    }
    
    String& append(const std::string& str) {
        m_str = bs_string_append_cstr(m_str, str.c_str());
        return *this;
    }
    
    String operator+(const String& other) const {
        bs_string* result = bs_string_concat(m_str, other.m_str);
        return String(result);
    }
    
    String operator+(const char* str) const {
        bs_string* temp = bs_string_new(str);
        bs_string* result = bs_string_concat(m_str, temp);
        bs_string_free(temp);
        return String(result);
    }
    
    String operator+(const std::string& str) const {
        bs_string* temp = bs_string_new_len(str.c_str(), str.length());
        bs_string* result = bs_string_concat(m_str, temp);
        bs_string_free(temp);
        return String(result);
    }
    
    // 修改
    String& insert(size_t position, const String& str) {
        bs_string* result = bs_string_insert(m_str, position, str.m_str);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    String& erase(size_t position, size_t length = std::string::npos) {
        if (length == std::string::npos) {
            length = this->length() - position;
        }
        bs_string* result = bs_string_delete(m_str, position, length);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    String& replace(const String& from, const String& to) {
        bs_string* result = bs_string_replace(m_str, from.m_str, to.m_str);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    String& replaceAll(const String& from, const String& to) {
        // 简单实现，循环替换所有匹配
        size_t start_pos = 0;
        while ((start_pos = find(from, start_pos)) != std::string::npos) {
            replace(from, to);
            start_pos += to.length();
        }
        return *this;
    }
    
    /**
     * 将字符串每个单词的首字母大写，其余字母小写
     * @return 对当前字符串的引用，用于链式调用
     */
    String& capitalize() {
        bs_string* result = bs_string_capitalize(m_str);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    /**
     * 反转字符串内容
     * @return 对当前字符串的引用，用于链式调用
     */
    String& reverse() {
        bs_string* result = bs_string_reverse(m_str);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    // 比较
    bool operator==(const String& other) const {
        return bs_string_equals(m_str, other.m_str);
    }
    
    bool operator!=(const String& other) const {
        return !bs_string_equals(m_str, other.m_str);
    }
    
    bool operator<(const String& other) const {
        return bs_string_compare(m_str, other.m_str) < 0;
    }
    
    bool operator<=(const String& other) const {
        return bs_string_compare(m_str, other.m_str) <= 0;
    }
    
    bool operator>(const String& other) const {
        return bs_string_compare(m_str, other.m_str) > 0;
    }
    
    bool operator>=(const String& other) const {
        return bs_string_compare(m_str, other.m_str) >= 0;
    }
    
    int compare(const String& other) const {
        return bs_string_compare(m_str, other.m_str);
    }
    
    bool equals(const String& other, bool ignoreCase = false) const {
        if (ignoreCase) {
            return bs_string_equals_ignore_case(m_str, other.m_str);
        } else {
            return bs_string_equals(m_str, other.m_str);
        }
    }
    
    // 查找
    size_t find(const String& str, size_t startPos = 0) const {
        int64_t result = bs_string_find(m_str, str.m_str, startPos);
        return result >= 0 ? static_cast<size_t>(result) : std::string::npos;
    }
    
    size_t find(const char* str, size_t startPos = 0) const {
        int64_t result = bs_string_find_cstr(m_str, str, startPos);
        return result >= 0 ? static_cast<size_t>(result) : std::string::npos;
    }
    
    size_t rfind(const String& str, size_t startPos = std::string::npos) const {
        int64_t result = bs_string_rfind(m_str, str.m_str, startPos);
        return result >= 0 ? static_cast<size_t>(result) : std::string::npos;
    }
    
    bool contains(const String& str) const {
        return bs_string_contains(m_str, str.m_str);
    }
    
    bool contains(const char* str) const {
        return bs_string_contains_cstr(m_str, str);
    }
    
    bool startsWith(const String& prefix) const {
        return bs_string_starts_with(m_str, prefix.m_str);
    }
    
    bool endsWith(const String& suffix) const {
        return bs_string_ends_with(m_str, suffix.m_str);
    }
    
    // 修饰
    String& trim() {
        bs_string* result = bs_string_trim(m_str);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    String& trimLeft() {
        bs_string* result = bs_string_trim_start(m_str);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    String& trimRight() {
        bs_string* result = bs_string_trim_end(m_str);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    // 修改操作
    String& toLower() {
        bs_string* result = bs_string_to_lower(m_str);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    String& toUpper() {
        bs_string* result = bs_string_to_upper(m_str);
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    // 非修改操作的副本版本
    String toLowerCopy() const {
        return String(bs_string_to_lower(m_str));
    }
    
    String toUpperCopy() const {
        return String(bs_string_to_upper(m_str));
    }
    
    String capitalizeCopy() const {
        return String(bs_string_capitalize(m_str));
    }
    
    String reverseCopy() const {
        return String(bs_string_reverse(m_str));
    }
    
    String trimCopy() const {
        return String(bs_string_trim(m_str));
    }
    
    // 分割与连接
    std::vector<String> split(const String& delimiter) const {
        bs_string_array* array = bs_string_split(m_str, delimiter.c_str());
        
        std::vector<String> result;
        if (array) {
            for (size_t i = 0; i < array->length; ++i) {
                result.emplace_back(String(array->items[i], true));
                array->items[i] = nullptr; // 防止数组释放字符串
            }
            bs_string_array_free(array);
        }
        
        return result;
    }
    
    std::vector<String> split(const char* delimiter) const {
        bs_string_array* array = bs_string_split(m_str, delimiter);
        
        std::vector<String> result;
        if (array) {
            for (size_t i = 0; i < array->length; ++i) {
                result.emplace_back(String(array->items[i], true));
                array->items[i] = nullptr; // 防止数组释放字符串
            }
            bs_string_array_free(array);
        }
        
        return result;
    }
    
    static String join(const std::vector<String>& strings, const String& delimiter) {
        if (strings.empty()) {
            return String();
        }
        
        String result = strings[0];
        for (size_t i = 1; i < strings.size(); ++i) {
            result.append(delimiter).append(strings[i]);
        }
        
        return result;
    }
    
    // 字符级操作
    char32_t charAt(size_t index) const {
        return bs_string_char_at(m_str, index);
    }
    
    String charAtAsString(size_t index) const {
        bs_string* result = bs_string_char_at_str(m_str, index);
        return String(result);
    }
    
    // 格式化实现
    static String format(const char* format, ...) {
        va_list args;
        va_start(args, format);
        bs_string* result = bs_string_format(format, args);
        va_end(args);
        return String(result);
    }
    
    // 数值转换实现
    static String fromInt(int64_t value) {
        bs_string* result = bs_string_from_int(value);
        return String(result);
    }
    
    static String fromFloat(double value) {
        bs_string* result = bs_string_from_float(value);
        return String(result);
    }
    
    int64_t toInt() const {
        return bs_string_to_int(m_str);
    }
    
    double toFloat() const {
        return bs_string_to_float(m_str);
    }
    
    bool tryParseInt(int64_t& value) const {
        return bs_string_try_parse_int(m_str, &value);
    }
    
    bool tryParseFloat(double& value) const {
        return bs_string_try_parse_float(m_str, &value);
    }
    
    // 正则表达式操作
    bool matches(const String& pattern) const {
        return bs_string_matches(m_str, pattern.c_str());
    }
    
    String& replaceRegex(const String& pattern, const String& replacement) {
        bs_string* result = bs_string_replace_regex(m_str, pattern.c_str(), replacement.c_str());
        if (result != m_str) {
            bs_string_free(m_str);
            m_str = result;
        }
        return *this;
    }
    
    std::vector<String> regexSplit(const String& pattern) const {
        bs_string_array* array = bs_string_regex_split(m_str, pattern.c_str());
        
        std::vector<String> result;
        if (array) {
            for (size_t i = 0; i < array->length; ++i) {
                result.emplace_back(String(array->items[i], true));
                array->items[i] = nullptr; // 防止数组释放字符串
            }
            bs_string_array_free(array);
        }
        
        return result;
    }
    
    std::vector<String> regexFindAll(const String& pattern) const {
        bs_string_array* array = bs_string_regex_find_all(m_str, pattern.c_str());
        
        std::vector<String> result;
        if (array) {
            for (size_t i = 0; i < array->length; ++i) {
                result.emplace_back(String(array->items[i], true));
                array->items[i] = nullptr; // 防止数组释放字符串
            }
            bs_string_array_free(array);
        }
        
        return result;
    }
    
    // 获取底层C字符串指针（高级用法）
    bs_string* getCString() const {
        return m_str;
    }
    
    // STL兼容性 - 迭代器支持
    class CharIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = char32_t;
        using pointer = char32_t*;
        using reference = char32_t;
        
        CharIterator(const bs_string* str, size_t pos = 0) : m_str(str), m_pos(pos), m_byte_pos(0) {
            if (str && pos > 0) {
                // 计算初始字节位置
                const uint8_t* data = str->data;
                size_t char_count = 0;
                while (char_count < pos && m_byte_pos < str->byte_length) {
                    m_byte_pos += utf8_char_size(data + m_byte_pos);
                    char_count++;
                }
            }
        }
        
        CharIterator& operator++() {
            if (m_str && m_pos < m_str->length) {
                m_byte_pos += utf8_char_size(m_str->data + m_byte_pos);
                m_pos++;
            }
            return *this;
        }
        
        CharIterator operator++(int) {
            CharIterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        CharIterator& operator--() {
            if (m_str && m_pos > 0) {
                m_pos--;
                // 需要从头开始找到前一个字符的位置
                size_t new_byte_pos = 0;
                size_t char_count = 0;
                while (char_count < m_pos) {
                    new_byte_pos += utf8_char_size(m_str->data + new_byte_pos);
                    char_count++;
                }
                m_byte_pos = new_byte_pos;
            }
            return *this;
        }
        
        CharIterator operator--(int) {
            CharIterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        reference operator*() const {
            if (m_str && m_pos < m_str->length) {
                return utf8_decode_char(m_str->data + m_byte_pos);
            }
            return 0;
        }
        
        bool operator==(const CharIterator& other) const {
            return m_str == other.m_str && m_pos == other.m_pos;
        }
        
        bool operator!=(const CharIterator& other) const {
            return !(*this == other);
        }
        
    private:
        const bs_string* m_str;
        size_t m_pos;      // 字符位置
        size_t m_byte_pos; // 字节位置
    };
    
    // 迭代器方法
    CharIterator begin() const {
        return CharIterator(m_str, 0);
    }
    
    CharIterator end() const {
        return CharIterator(m_str, length());
    }
    
    // STL兼容性 - 容器接口
    char32_t at(size_t pos) const {
        if (pos >= length()) {
            throw std::out_of_range("String index out of range");
        }
        return charAt(pos);
    }
    
    void push_back(char32_t ch) {
        uint8_t buffer[4];
        size_t size = utf8_encode_char(ch, buffer);
        buffer[size] = '\0';
        append(reinterpret_cast<char*>(buffer));
    }
    
    void pop_back() {
        if (isEmpty()) return;
        
        // 创建去掉最后一个字符的新字符串
        size_t len = length();
        bs_string* result = bs_string_substring(m_str, 0, len - 1);
        bs_string_free(m_str);
        m_str = result;
    }
    
    void clear() {
        freeString();
        m_str = bs_string_new("");
    }
    
    size_t size() const {
        return length();
    }
    
    bool empty() const {
        return isEmpty();
    }

private:
    bs_string* m_str; // 内部使用C版本的字符串类型
    
    // 内部工具方法
    void freeString() {
        if (m_str) {
            bs_string_free(m_str);
            m_str = nullptr;
        }
    }
};

// 非成员函数定义
inline String operator+(const char* lhs, const String& rhs) {
    return String(lhs) + rhs;
}

inline String operator+(const std::string& lhs, const String& rhs) {
    return String(lhs) + rhs;
}

inline std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.c_str();
    return os;
}

inline std::istream& operator>>(std::istream& is, String& str) {
    std::string temp;
    is >> temp;
    str = temp;
    return is;
}

// 添加STL兼容的哈希支持
namespace std {
    template <>
    struct hash<bs::String> {
        size_t operator()(const bs::String& str) const {
            // 使用FNV-1a哈希算法
            const uint8_t* data = reinterpret_cast<const uint8_t*>(str.c_str());
            size_t hash = 14695981039346656037ULL; // FNV偏移基数
            for (size_t i = 0; i < str.byteLength(); ++i) {
                hash ^= static_cast<size_t>(data[i]);
                hash *= 1099511628211ULL; // FNV素数
            }
            return hash;
        }
    };
}

} // namespace bs

#endif // BETTER_STRING_SINGLE_HPP 