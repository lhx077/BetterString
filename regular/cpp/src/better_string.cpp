/**
 * @file better_string.cpp
 * @brief BetterString C++库实现
 *
 * 本文件实现了BetterString的C++接口
 */

#include "better_string.hpp"
#include <iostream>
#include <cstring>
#include <algorithm>

namespace bs {

// 构造函数
String::String() {
#ifdef BETTER_STRING_USE_C_BACKEND
    m_handle = bs_create_empty();
#else
    m_length = 0;
    m_capacity = 16;
    m_data = std::make_unique<char[]>(m_capacity);
    m_data[0] = '\0';
#endif
}

String::String(const char* str) {
#ifdef BETTER_STRING_USE_C_BACKEND
    m_handle = bs_create(str);
#else
    m_length = strlen(str);
    m_capacity = m_length + 1;
    m_data = std::make_unique<char[]>(m_capacity);
    strcpy(m_data.get(), str);
#endif
}

String::String(const std::string& str) {
#ifdef BETTER_STRING_USE_C_BACKEND
    m_handle = bs_create(str.c_str());
#else
    m_length = str.length();
    m_capacity = m_length + 1;
    m_data = std::make_unique<char[]>(m_capacity);
    strcpy(m_data.get(), str.c_str());
#endif
}

String::String(const String& other) {
#ifdef BETTER_STRING_USE_C_BACKEND
    m_handle = bs_clone(other.m_handle);
#else
    m_length = other.m_length;
    m_capacity = other.m_capacity;
    m_data = std::make_unique<char[]>(m_capacity);
    strcpy(m_data.get(), other.m_data.get());
#endif
}

String::String(String&& other) noexcept {
#ifdef BETTER_STRING_USE_C_BACKEND
    m_handle = other.m_handle;
    other.m_handle = nullptr;
#else
    m_length = other.m_length;
    m_capacity = other.m_capacity;
    m_data = std::move(other.m_data);
    other.m_length = 0;
    other.m_capacity = 0;
#endif
}

String::~String() {
#ifdef BETTER_STRING_USE_C_BACKEND
    if (m_handle) {
        bs_destroy(m_handle);
    }
#endif
}

// 基本操作示例实现
String& String::append(const char* str) {
#ifdef BETTER_STRING_USE_C_BACKEND
    bs_append(m_handle, str);
#else
    size_t addLen = strlen(str);
    size_t newLen = m_length + addLen;
    
    if (newLen >= m_capacity) {
        size_t newCapacity = std::max(m_capacity * 2, newLen + 1);
        auto newData = std::make_unique<char[]>(newCapacity);
        strcpy(newData.get(), m_data.get());
        m_data = std::move(newData);
        m_capacity = newCapacity;
    }
    
    strcpy(m_data.get() + m_length, str);
    m_length = newLen;
#endif
    return *this;
}

// 示例实现 - toUpper
String& String::toUpper() {
#ifdef BETTER_STRING_USE_C_BACKEND
    bs_to_upper(m_handle);
#else
    for (size_t i = 0; i < m_length; i++) {
        if (m_data[i] >= 'a' && m_data[i] <= 'z') {
            m_data[i] = m_data[i] - 'a' + 'A';
        }
    }
#endif
    return *this;
}

// 新增方法 - capitalize
String& String::capitalize() {
#ifdef BETTER_STRING_USE_C_BACKEND
    bs_string* result = bs_string_capitalize(m_handle);
    if (result != m_handle) {
        bs_string_free(m_handle);
        m_handle = result;
    }
#else
    bool capitalize_next = true;
    
    for (size_t i = 0; i < m_length; i++) {
        if (std::isalpha(m_data[i])) {
            if (capitalize_next) {
                m_data[i] = std::toupper(m_data[i]);
                capitalize_next = false;
            } else {
                m_data[i] = std::tolower(m_data[i]);
            }
        } else if (std::isspace(m_data[i]) || std::ispunct(m_data[i])) {
            capitalize_next = true;
        }
    }
#endif
    return *this;
}

// 新增方法 - reverse
String& String::reverse() {
#ifdef BETTER_STRING_USE_C_BACKEND
    bs_string* result = bs_string_reverse(m_handle);
    if (result != m_handle) {
        bs_string_free(m_handle);
        m_handle = result;
    }
#else
    // 简单实现，仅针对ASCII字符串
    // 实际实现应该考虑UTF-8等多字节编码
    size_t start = 0;
    size_t end = m_length - 1;
    
    while (start < end) {
        char temp = m_data[start];
        m_data[start] = m_data[end];
        m_data[end] = temp;
        start++;
        end--;
    }
#endif
    return *this;
}

// 新增方法 - wordCount
size_t String::wordCount() const {
#ifdef BETTER_STRING_USE_C_BACKEND
    return bs_string_word_count(m_handle);
#else
    size_t count = 0;
    bool in_word = false;
    
    for (size_t i = 0; i < m_length; i++) {
        bool is_space = std::isspace(m_data[i]);
        
        if (!is_space && !in_word) {
            count++;
        }
        
        in_word = !is_space;
    }
    
    return count;
#endif
}

// 长度访问
size_t String::length() const {
#ifdef BETTER_STRING_USE_C_BACKEND
    return bs_length(m_handle);
#else
    return m_length;
#endif
}

// C字符串访问
const char* String::c_str() const {
#ifdef BETTER_STRING_USE_C_BACKEND
    return bs_c_str(m_handle);
#else
    return m_data.get();
#endif
}

// 流操作符
std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.c_str();
    return os;
}

// 迭代器实现
String::CharIterator::CharIterator(const String* str, size_t pos) 
    : m_str(str), m_pos(pos), m_byte_pos(0) {
    if (str && pos > 0) {
        // 计算字节位置
        // 需要通过低级API访问
        // 此处简化实现，实际实现需要通过C API访问底层数据
    }
}

String::CharIterator& String::CharIterator::operator++() {
    if (m_str && m_pos < m_str->length()) {
        // 移动到下一个字符
        m_pos++;
        // 更新字节位置
    }
    return *this;
}

String::CharIterator String::CharIterator::operator++(int) {
    CharIterator tmp = *this;
    ++(*this);
    return tmp;
}

String::CharIterator& String::CharIterator::operator--() {
    if (m_str && m_pos > 0) {
        m_pos--;
        // 更新字节位置
    }
    return *this;
}

String::CharIterator String::CharIterator::operator--(int) {
    CharIterator tmp = *this;
    --(*this);
    return tmp;
}

String::CharIterator::reference String::CharIterator::operator*() const {
    if (m_str && m_pos < m_str->length()) {
        return m_str->charAt(m_pos);
    }
    return 0;
}

bool String::CharIterator::operator==(const CharIterator& other) const {
    return m_str == other.m_str && m_pos == other.m_pos;
}

bool String::CharIterator::operator!=(const CharIterator& other) const {
    return !(*this == other);
}

// 迭代器方法
String::CharIterator String::begin() const {
    return CharIterator(this, 0);
}

String::CharIterator String::end() const {
    return CharIterator(this, length());
}

// STL容器接口
char32_t String::at(size_t pos) const {
    if (pos >= length()) {
        throw std::out_of_range("String index out of range");
    }
    return charAt(pos);
}

void String::push_back(char32_t ch) {
    // 将Unicode字符转换为UTF-8并追加
    char buffer[5] = {0};
    // 转换逻辑
    append(buffer);
}

void String::pop_back() {
    if (!empty()) {
        // 移除最后一个字符
        // 实现逻辑
    }
}

void String::clear() {
    // 清空字符串
    // 实现逻辑
}

size_t String::size() const {
    return length();
}

bool String::empty() const {
    return length() == 0;
}

// 哈希函数实现
size_t std::hash<bs::String>::operator()(const bs::String& str) const {
    // 使用FNV-1a哈希算法或其他适合的哈希算法
    size_t hash = 14695981039346656037ULL; // FNV偏移基数
    const char* data = str.c_str();
    for (size_t i = 0; data[i]; ++i) {
        hash ^= static_cast<size_t>(data[i]);
        hash *= 1099511628211ULL; // FNV素数
    }
    return hash;
}

} // namespace bs 