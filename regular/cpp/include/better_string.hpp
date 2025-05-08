/**
 * @file better_string.hpp
 * @brief BetterString C++库头文件
 *
 * 本文件定义了BetterString的C++接口
 */

#ifndef BETTER_STRING_HPP
#define BETTER_STRING_HPP

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

// 根据配置决定是否使用C库作为后端
#ifdef BETTER_STRING_USE_C_BACKEND
#include "better_string.h"
#endif

namespace bs {

/**
 * @brief 高性能字符串类
 */
class String {
public:
    // 构造函数
    String();
    String(const char* str);
    String(const std::string& str);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    // 赋值操作符
    String& operator=(const char* str);
    String& operator=(const std::string& str);
    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;

    // STL兼容性 - 迭代器支持
    class CharIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = char32_t;
        using pointer = char32_t*;
        using reference = char32_t;
        
        CharIterator(const String* str, size_t pos = 0);
        CharIterator& operator++();
        CharIterator operator++(int);
        CharIterator& operator--();
        CharIterator operator--(int);
        reference operator*() const;
        bool operator==(const CharIterator& other) const;
        bool operator!=(const CharIterator& other) const;
        
    private:
        const String* m_str;
        size_t m_pos;
        size_t m_byte_pos;
    };
    
    // 迭代器方法
    CharIterator begin() const;
    CharIterator end() const;
    
    // STL兼容性 - 容器接口
    char32_t at(size_t pos) const;
    void push_back(char32_t ch);
    void pop_back();
    void clear();
    size_t size() const;
    bool empty() const;

    // 基本操作
    String& append(const char* str);
    String& append(const String& other);
    String& insert(size_t pos, const char* str);
    String& insert(size_t pos, const String& other);
    String& replace(size_t pos, size_t len, const char* str);
    String& erase(size_t pos, size_t len);
    
    // 查找操作
    size_t find(const char* str, size_t pos = 0) const;
    size_t find(const String& other, size_t pos = 0) const;
    bool contains(const char* str) const;
    bool startsWith(const char* prefix) const;
    bool endsWith(const char* suffix) const;
    
    // 转换操作
    String& toUpper();
    String& toLower();
    String& trim();
    String& trimLeft();
    String& trimRight();
    /**
     * 将字符串中每个单词的首字母大写，其余字母小写
     * @return 对当前字符串的引用，用于链式调用
     */
    String& capitalize();
    /**
     * 反转字符串内容
     * @return 对当前字符串的引用，用于链式调用
     */
    String& reverse();
    
    // 分割
    std::vector<String> split(const char* delimiter) const;
    
    // 长度和容量
    size_t length() const;
    size_t capacity() const;
    /**
     * 计算字符串中单词的数量
     * @return 单词数量
     */
    size_t wordCount() const;
    void reserve(size_t capacity);
    
    // 访问
    char operator[](size_t pos) const;
    char& operator[](size_t pos);
    const char* c_str() const;
    std::string str() const;
    
    // 链式调用支持
    String& chain();
    String& endChain();

private:
#ifdef BETTER_STRING_USE_C_BACKEND
    // 使用C版本作为后端
    bs_string_t* m_handle;
#else
    // 独立实现
    std::unique_ptr<char[]> m_data;
    size_t m_length;
    size_t m_capacity;
#endif
};

// 流操作符
std::ostream& operator<<(std::ostream& os, const String& str);

// 添加STL兼容的哈希支持
namespace std {
    template <>
    struct hash<bs::String> {
        size_t operator()(const bs::String& str) const;
    };
}

} // namespace bs

#endif // BETTER_STRING_HPP 