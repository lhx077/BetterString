/**
 * better_string_single.h - 高性能字符串库的C语言实现 (单文件版本)
 * 
 * 这个库提供了一个高性能、功能丰富的字符串处理库，
 * 支持Unicode和各种现代字符串操作。
 */

#ifndef BETTER_STRING_SINGLE_H
#define BETTER_STRING_SINGLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 字符串编码类型
 */
typedef enum {
    BS_ENCODING_UTF8,    // UTF-8编码
    BS_ENCODING_UTF16,   // UTF-16编码
    BS_ENCODING_UTF32,   // UTF-32编码
    BS_ENCODING_ASCII    // ASCII编码
} bs_encoding_t;

/**
 * 字符串对象类型
 */
typedef struct bs_string {
    uint8_t* data;           // 原始字节数据
    size_t length;           // 字符数量（不是字节长度）
    size_t byte_length;      // 字节长度
    bs_encoding_t encoding;  // 编码类型
    bool is_view;            // 是否为视图（不拥有内存）
} bs_string;

/**
 * 字符串数组类型
 */
typedef struct bs_string_array {
    bs_string** items;       // 字符串数组
    size_t length;           // 数组长度
} bs_string_array;

/**
 * 迭代器类型
 */
typedef struct bs_iterator {
    const bs_string* str;    // 被迭代的字符串
    size_t position;         // 当前字节位置
    size_t char_position;    // 当前字符位置
} bs_iterator;

// 前向声明所有函数
bs_string* bs_string_new(const char* str);
bs_string* bs_string_new_len(const char* str, size_t length);
bs_string* bs_string_new_utf16(const uint16_t* str);
bs_string* bs_string_new_utf32(const uint32_t* str);
bs_string* bs_string_new_with_encoding(const void* str, size_t byte_length, bs_encoding_t encoding);
bs_string* bs_string_copy(const bs_string* str);
void bs_string_free(bs_string* str);
size_t bs_string_length(const bs_string* str);
size_t bs_string_byte_length(const bs_string* str);
bs_encoding_t bs_string_encoding(const bs_string* str);
const char* bs_string_c_str(const bs_string* str);
bs_string* bs_string_substring(const bs_string* str, size_t start, size_t length);
bs_string* bs_string_concat(const bs_string* str1, const bs_string* str2);
bs_string* bs_string_append(bs_string* str, const bs_string* append);
bs_string* bs_string_append_cstr(bs_string* str, const char* append);
bs_string* bs_string_insert(bs_string* str, size_t position, const bs_string* insert);
bs_string* bs_string_delete(bs_string* str, size_t start, size_t length);
bs_string* bs_string_replace(bs_string* str, const bs_string* find, const bs_string* replace);
bs_string* bs_string_trim(const bs_string* str);
bs_string* bs_string_trim_start(const bs_string* str);
bs_string* bs_string_trim_end(const bs_string* str);
bs_string* bs_string_to_upper(const bs_string* str);
bs_string* bs_string_to_lower(const bs_string* str);
/**
 * 将字符串首字母大写（每个单词的首字母变为大写，其余为小写）
 * @param str 源字符串
 * @return 新的字符串，包含首字母大写的结果
 */
bs_string* bs_string_capitalize(const bs_string* str);
/**
 * 反转字符串内容
 * @param str 源字符串
 * @return 新的字符串，包含反转后的内容
 */
bs_string* bs_string_reverse(const bs_string* str);
int64_t bs_string_find(const bs_string* str, const bs_string* find, size_t start_pos);
int64_t bs_string_find_cstr(const bs_string* str, const char* find, size_t start_pos);
int64_t bs_string_rfind(const bs_string* str, const bs_string* find, size_t start_pos);
bool bs_string_contains(const bs_string* str, const bs_string* find);
bool bs_string_contains_cstr(const bs_string* str, const char* find);
bool bs_string_starts_with(const bs_string* str, const bs_string* prefix);
bool bs_string_ends_with(const bs_string* str, const bs_string* suffix);
int bs_string_compare(const bs_string* str1, const bs_string* str2);
bool bs_string_equals(const bs_string* str1, const bs_string* str2);
bool bs_string_equals_ignore_case(const bs_string* str1, const bs_string* str2);
bs_string_array* bs_string_split(const bs_string* str, const char* delimiter);
bs_string* bs_string_join(const bs_string_array* array, const char* delimiter);
void bs_string_array_free(bs_string_array* array);
/**
 * 计算字符串中单词的数量
 * @param str 源字符串
 * @return 字符串中单词的数量
 */
size_t bs_string_word_count(const bs_string* str);
uint32_t bs_string_char_at(const bs_string* str, size_t index);
bs_string* bs_string_char_at_str(const bs_string* str, size_t index);
bs_iterator* bs_string_iterator_new(const bs_string* str);
bool bs_string_iterator_has_next(const bs_iterator* it);
uint32_t bs_string_iterator_next(bs_iterator* it);
void bs_string_iterator_free(bs_iterator* it);
bs_string* bs_string_from_int(int64_t value);
bs_string* bs_string_from_float(double value);
int64_t bs_string_to_int(const bs_string* str);
double bs_string_to_float(const bs_string* str);
bool bs_string_try_parse_int(const bs_string* str, int64_t* value);
bool bs_string_try_parse_float(const bs_string* str, double* value);
bool bs_string_matches(const bs_string* str, const char* pattern);
bs_string* bs_string_replace_regex(const bs_string* str, const char* pattern, const char* replacement);
bs_string_array* bs_string_regex_split(const bs_string* str, const char* pattern);
bs_string_array* bs_string_regex_find_all(const bs_string* str, const char* pattern);
bs_string* bs_string_format(const char* format, ...);

// 内部工具函数声明
static size_t utf8_strlen(const char* str, size_t byte_length);
static size_t utf8_char_size(const uint8_t* str);
static uint32_t utf8_decode_char(const uint8_t* str);
static size_t utf8_encode_char(uint32_t ch, uint8_t* buffer);
static bs_string* bs_string_alloc(size_t byte_length, bs_encoding_t encoding);

#ifdef BETTER_STRING_IMPLEMENTATION

// 内部工具函数实现

static bs_string* bs_string_alloc(size_t byte_length, bs_encoding_t encoding) {
    bs_string* str = (bs_string*)malloc(sizeof(bs_string));
    if (!str) {
        return NULL;
    }
    
    str->data = (uint8_t*)malloc(byte_length + 1); // 额外的空间用于null终止符
    if (!str->data) {
        free(str);
        return NULL;
    }
    
    str->data[byte_length] = '\0'; // 确保null终止
    str->byte_length = byte_length;
    str->length = 0; // 初始长度为0，会在具体构造函数中设置
    str->encoding = encoding;
    str->is_view = false;
    
    return str;
}

static size_t utf8_strlen(const char* str, size_t byte_length) {
    if (!str || byte_length == 0) {
        return 0;
    }
    
    size_t length = 0;
    const uint8_t* data = (const uint8_t*)str;
    size_t i = 0;
    
    while (i < byte_length) {
        i += utf8_char_size(data + i);
        length++;
    }
    
    return length;
}

static size_t utf8_char_size(const uint8_t* str) {
    if (!str || !*str) {
        return 0;
    }
    
    if ((*str & 0x80) == 0) {
        return 1; // ASCII字符
    } else if ((*str & 0xE0) == 0xC0) {
        return 2; // 2字节UTF-8
    } else if ((*str & 0xF0) == 0xE0) {
        return 3; // 3字节UTF-8
    } else if ((*str & 0xF8) == 0xF0) {
        return 4; // 4字节UTF-8
    }
    
    // 无效的UTF-8序列，假设为1字节
    return 1;
}

static uint32_t utf8_decode_char(const uint8_t* str) {
    if (!str || !*str) {
        return 0;
    }
    
    size_t size = utf8_char_size(str);
    uint32_t ch = 0;
    
    switch (size) {
        case 1:
            return *str;
        case 2:
            ch = ((str[0] & 0x1F) << 6) | (str[1] & 0x3F);
            break;
        case 3:
            ch = ((str[0] & 0x0F) << 12) | ((str[1] & 0x3F) << 6) | (str[2] & 0x3F);
            break;
        case 4:
            ch = ((str[0] & 0x07) << 18) | ((str[1] & 0x3F) << 12) | ((str[2] & 0x3F) << 6) | (str[3] & 0x3F);
            break;
        default:
            return 0; // 无效的UTF-8序列
    }
    
    return ch;
}

static size_t utf8_encode_char(uint32_t ch, uint8_t* buffer) {
    if (!buffer) {
        return 0;
    }
    
    if (ch < 0x80) {
        // ASCII字符
        buffer[0] = (uint8_t)ch;
        return 1;
    } else if (ch < 0x800) {
        // 2字节UTF-8
        buffer[0] = 0xC0 | (ch >> 6);
        buffer[1] = 0x80 | (ch & 0x3F);
        return 2;
    } else if (ch < 0x10000) {
        // 3字节UTF-8
        buffer[0] = 0xE0 | (ch >> 12);
        buffer[1] = 0x80 | ((ch >> 6) & 0x3F);
        buffer[2] = 0x80 | (ch & 0x3F);
        return 3;
    } else if (ch < 0x110000) {
        // 4字节UTF-8
        buffer[0] = 0xF0 | (ch >> 18);
        buffer[1] = 0x80 | ((ch >> 12) & 0x3F);
        buffer[2] = 0x80 | ((ch >> 6) & 0x3F);
        buffer[3] = 0x80 | (ch & 0x3F);
        return 4;
    }
    
    // 无效的Unicode代码点
    buffer[0] = '?';
    return 1;
}

// 修饰方法实现
bs_string* bs_string_trim(const bs_string* str) {
    if (!str || !str->data || str->byte_length == 0) {
        return bs_string_new("");
    }
    
    // 找到开始和结束的非空白字符位置
    size_t start = 0;
    size_t end = str->byte_length;
    const char* cstr = (const char*)str->data;
    
    // 查找开始位置
    while (start < end && isspace(cstr[start])) {
        start++;
    }
    
    // 查找结束位置
    while (end > start && isspace(cstr[end - 1])) {
        end--;
    }
    
    // 创建新字符串
    return bs_string_new_len(cstr + start, end - start);
}

bs_string* bs_string_trim_start(const bs_string* str) {
    if (!str || !str->data || str->byte_length == 0) {
        return bs_string_new("");
    }
    
    // 找到开始的非空白字符位置
    size_t start = 0;
    const char* cstr = (const char*)str->data;
    
    // 查找开始位置
    while (start < str->byte_length && isspace(cstr[start])) {
        start++;
    }
    
    // 创建新字符串
    return bs_string_new_len(cstr + start, str->byte_length - start);
}

bs_string* bs_string_trim_end(const bs_string* str) {
    if (!str || !str->data || str->byte_length == 0) {
        return bs_string_new("");
    }
    
    // 找到结束的非空白字符位置
    size_t end = str->byte_length;
    const char* cstr = (const char*)str->data;
    
    // 查找结束位置
    while (end > 0 && isspace(cstr[end - 1])) {
        end--;
    }
    
    // 创建新字符串
    return bs_string_new_len(cstr, end);
}

bs_string* bs_string_to_upper(const bs_string* str) {
    if (!str) {
        return NULL;
    }
    
    // 创建一个新的字符串作为结果
    bs_string* result = bs_string_copy(str);
    if (!result) {
        return NULL;
    }
    
    // 对ASCII字符执行大写转换
    char* data = (char*)result->data;
    for (size_t i = 0; i < result->byte_length; i++) {
        // 只对ASCII字符进行转换
        if ((data[i] & 0x80) == 0 && islower(data[i])) {
            data[i] = toupper(data[i]);
        }
    }
    
    return result;
}

bs_string* bs_string_to_lower(const bs_string* str) {
    if (!str) {
        return NULL;
    }
    
    // 创建一个新的字符串作为结果
    bs_string* result = bs_string_copy(str);
    if (!result) {
        return NULL;
    }
    
    // 对ASCII字符执行小写转换
    char* data = (char*)result->data;
    for (size_t i = 0; i < result->byte_length; i++) {
        // 只对ASCII字符进行转换
        if ((data[i] & 0x80) == 0 && isupper(data[i])) {
            data[i] = tolower(data[i]);
        }
    }
    
    return result;
}

// 简单的正则表达式操作实现
// 注意：这是非常基础的实现，只支持简单的模式匹配
// 完整实现应该使用成熟的正则表达式库

bool bs_string_matches(const bs_string* str, const char* pattern) {
    if (!str || !pattern) {
        return false;
    }
    
    // 简单实现：使用基础的字符串匹配
    // 只支持 * 和 ? 通配符
    
    const char* s = (const char*)str->data;
    const char* p = pattern;
    
    // 特殊情况：空字符串
    if (str->byte_length == 0) {
        return *p == '\0';
    }
    
    // 特殊情况：空模式
    if (*p == '\0') {
        return str->byte_length == 0;
    }
    
    // 简单模式匹配
    if (*p == '*') {
        // * 可以匹配0个或多个字符
        while (*(p+1) == '*') p++; // 跳过连续的*
        
        if (*(p+1) == '\0') {
            return true; // 如果模式以*结尾，则匹配任何剩余字符
        }
        
        const char* star_pattern = p + 1;
        for (size_t i = 0; i <= str->byte_length; i++) {
            bs_string* substr = bs_string_new_len(s + i, str->byte_length - i);
            bool result = bs_string_matches(substr, star_pattern);
            bs_string_free(substr);
            if (result) {
                return true;
            }
        }
        return false;
    } else if (*p == '?') {
        // ? 匹配任何单个字符
        return str->byte_length > 0 && bs_string_matches(
            bs_string_new_len(s + 1, str->byte_length - 1), 
            p + 1
        );
    } else {
        // 普通字符匹配
        return str->byte_length > 0 && *s == *p && bs_string_matches(
            bs_string_new_len(s + 1, str->byte_length - 1), 
            p + 1
        );
    }
}

bs_string* bs_string_replace_regex(const bs_string* str, const char* pattern, const char* replacement) {
    if (!str || !pattern || !replacement) {
        return bs_string_copy(str);
    }
    
    // 简单实现：只支持直接子字符串替换
    bs_string* pattern_str = bs_string_new(pattern);
    bs_string* replacement_str = bs_string_new(replacement);
    
    bs_string* result = bs_string_replace(str, pattern_str, replacement_str);
    
    bs_string_free(pattern_str);
    bs_string_free(replacement_str);
    
    return result;
}

bs_string_array* bs_string_regex_split(const bs_string* str, const char* pattern) {
    if (!str || !pattern) {
        return NULL;
    }
    
    // 简单实现：直接使用字符串分割
    return bs_string_split(str, pattern);
}

bs_string_array* bs_string_regex_find_all(const bs_string* str, const char* pattern) {
    if (!str || !pattern) {
        return NULL;
    }
    
    // 简化实现：模式为简单子字符串的情况
    bs_string* pattern_str = bs_string_new(pattern);
    
    // 分配结果数组
    bs_string_array* result = (bs_string_array*)malloc(sizeof(bs_string_array));
    if (!result) {
        bs_string_free(pattern_str);
        return NULL;
    }
    
    // 查找所有匹配
    size_t pos = 0;
    size_t max_matches = 16; // 初始容量
    result->items = (bs_string**)malloc(max_matches * sizeof(bs_string*));
    result->length = 0;
    
    while (1) {
        int64_t match_pos = bs_string_find(str, pattern_str, pos);
        if (match_pos < 0) {
            break;
        }
        
        // 计算匹配的字节位置
        size_t byte_pos = 0;
        for (size_t i = 0; i < match_pos && byte_pos < str->byte_length; ) {
            byte_pos += utf8_char_size(str->data + byte_pos);
            i++;
        }
        
        // 添加匹配的子字符串
        size_t match_len = pattern_str->byte_length;
        bs_string* match = bs_string_new_len((const char*)(str->data + byte_pos), match_len);
        
        // 如果需要扩容
        if (result->length >= max_matches) {
            max_matches *= 2;
            bs_string** new_items = (bs_string**)realloc(result->items, max_matches * sizeof(bs_string*));
            if (!new_items) {
                // 内存分配失败，释放资源
                bs_string_free(match);
                for (size_t i = 0; i < result->length; i++) {
                    bs_string_free(result->items[i]);
                }
                free(result->items);
                free(result);
                bs_string_free(pattern_str);
                return NULL;
            }
            result->items = new_items;
        }
        
        result->items[result->length++] = match;
        
        // 移动到下一个位置
        pos = match_pos + 1;
    }
    
    bs_string_free(pattern_str);
    return result;
}

// 构造函数实现

bs_string* bs_string_new(const char* str) {
    if (!str) {
        return bs_string_new_len("", 0);
    }
    return bs_string_new_len(str, strlen(str));
}

bs_string* bs_string_new_len(const char* str, size_t byte_length) {
    if (!str) {
        str = "";
        byte_length = 0;
    }
    
    bs_string* result = bs_string_alloc(byte_length, BS_ENCODING_UTF8);
    if (!result) {
        return NULL;
    }
    
    memcpy(result->data, str, byte_length);
    result->byte_length = byte_length;
    result->length = utf8_strlen(str, byte_length);
    result->is_view = false;
    
    return result;
}

bs_string* bs_string_new_utf16(const uint16_t* str) {
    // 简化实现：先转换为UTF-8然后创建
    if (!str) {
        return bs_string_new_len("", 0);
    }
    
    // 计算UTF-16字符串长度
    size_t utf16_len = 0;
    while (str[utf16_len]) {
        utf16_len++;
    }
    
    // 估算UTF-8可能需要的最大长度（每个UTF-16字符最多3个UTF-8字节）
    size_t max_utf8_len = utf16_len * 3;
    uint8_t* utf8_buf = (uint8_t*)malloc(max_utf8_len + 1);
    if (!utf8_buf) {
        return NULL;
    }
    
    // 执行UTF-16到UTF-8的转换
    size_t utf8_pos = 0;
    for (size_t i = 0; i < utf16_len; i++) {
        uint32_t codepoint = str[i];
        
        // 处理代理对
        if (codepoint >= 0xD800 && codepoint <= 0xDBFF && i + 1 < utf16_len) {
            uint32_t low_surrogate = str[i + 1];
            if (low_surrogate >= 0xDC00 && low_surrogate <= 0xDFFF) {
                codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low_surrogate - 0xDC00);
                i++; // 跳过下一个字符，因为它是代理对的一部分
            }
        }
        
        // 编码为UTF-8
        utf8_pos += utf8_encode_char(codepoint, utf8_buf + utf8_pos);
    }
    
    utf8_buf[utf8_pos] = '\0';
    
    // 创建字符串
    bs_string* result = bs_string_new_len((char*)utf8_buf, utf8_pos);
    free(utf8_buf);
    
    return result;
}

bs_string* bs_string_new_utf32(const uint32_t* str) {
    // 实现与UTF-16类似，但处理更直接
    if (!str) {
        return bs_string_new_len("", 0);
    }
    
    // 计算UTF-32字符串长度
    size_t utf32_len = 0;
    while (str[utf32_len]) {
        utf32_len++;
    }
    
    // 估算UTF-8可能需要的最大长度（每个UTF-32字符最多4个UTF-8字节）
    size_t max_utf8_len = utf32_len * 4;
    uint8_t* utf8_buf = (uint8_t*)malloc(max_utf8_len + 1);
    if (!utf8_buf) {
        return NULL;
    }
    
    // 执行UTF-32到UTF-8的转换
    size_t utf8_pos = 0;
    for (size_t i = 0; i < utf32_len; i++) {
        utf8_pos += utf8_encode_char(str[i], utf8_buf + utf8_pos);
    }
    
    utf8_buf[utf8_pos] = '\0';
    
    // 创建字符串
    bs_string* result = bs_string_new_len((char*)utf8_buf, utf8_pos);
    free(utf8_buf);
    
    return result;
}

bs_string* bs_string_new_with_encoding(const void* str, size_t byte_length, bs_encoding_t encoding) {
    if (!str) {
        return bs_string_new_len("", 0);
    }
    
    switch (encoding) {
        case BS_ENCODING_UTF8:
            return bs_string_new_len((const char*)str, byte_length);
        
        case BS_ENCODING_UTF16:
            return bs_string_new_utf16((const uint16_t*)str);
        
        case BS_ENCODING_UTF32:
            return bs_string_new_utf32((const uint32_t*)str);
        
        case BS_ENCODING_ASCII:
            // ASCII可以直接当作UTF-8处理
            return bs_string_new_len((const char*)str, byte_length);
            
        default:
            return NULL;
    }
}

bs_string* bs_string_copy(const bs_string* str) {
    if (!str) {
        return NULL;
    }
    
    bs_string* result = bs_string_alloc(str->byte_length, str->encoding);
    if (!result) {
        return NULL;
    }
    
    memcpy(result->data, str->data, str->byte_length);
    result->byte_length = str->byte_length;
    result->length = str->length;
    result->is_view = false;
    
    return result;
}

void bs_string_free(bs_string* str) {
    if (str) {
        if (!str->is_view && str->data) {
            free(str->data);
        }
        free(str);
    }
}

// 字符串属性实现

size_t bs_string_length(const bs_string* str) {
    return str ? str->length : 0;
}

size_t bs_string_byte_length(const bs_string* str) {
    return str ? str->byte_length : 0;
}

bs_encoding_t bs_string_encoding(const bs_string* str) {
    return str ? str->encoding : BS_ENCODING_UTF8;
}

const char* bs_string_c_str(const bs_string* str) {
    if (!str) {
        return "";
    }
    
    // 确保字符串以null结尾
    if (str->byte_length > 0 && str->data[str->byte_length - 1] != '\0') {
        // 如果字符串不是以null结尾的，则创建一个新的
        bs_string* new_str = bs_string_alloc(str->byte_length + 1, str->encoding);
        if (!new_str) {
            return "";
        }
        
        memcpy(new_str->data, str->data, str->byte_length);
        new_str->data[str->byte_length] = '\0';
        
        // 这里简化了实现，实际上应该缓存结果
        return (const char*)new_str->data;
    }
    
    return (const char*)str->data;
}

// 字符串操作实现

bs_string* bs_string_substring(const bs_string* str, size_t start, size_t length) {
    if (!str || start >= str->length) {
        return bs_string_new("");
    }
    
    // 找到起始位置的字节偏移
    size_t start_byte = 0;
    size_t char_count = 0;
    const uint8_t* data = str->data;
    
    while (char_count < start && start_byte < str->byte_length) {
        start_byte += utf8_char_size(data + start_byte);
        char_count++;
    }
    
    // 如果length超出了可能的范围，就调整它
    if (start + length > str->length) {
        length = str->length - start;
    }
    
    // 找到结束位置的字节偏移
    size_t end_byte = start_byte;
    char_count = 0;
    
    while (char_count < length && end_byte < str->byte_length) {
        end_byte += utf8_char_size(data + end_byte);
        char_count++;
    }
    
    // 创建子字符串
    return bs_string_new_len((const char*)(data + start_byte), end_byte - start_byte);
}

bs_string* bs_string_concat(const bs_string* str1, const bs_string* str2) {
    if (!str1) {
        return str2 ? bs_string_copy(str2) : bs_string_new("");
    }
    if (!str2) {
        return bs_string_copy(str1);
    }
    
    // 分配足够大的空间来存储两个字符串
    bs_string* result = bs_string_alloc(str1->byte_length + str2->byte_length, BS_ENCODING_UTF8);
    if (!result) {
        return NULL;
    }
    
    // 复制两个字符串的内容
    memcpy(result->data, str1->data, str1->byte_length);
    memcpy(result->data + str1->byte_length, str2->data, str2->byte_length);
    
    result->byte_length = str1->byte_length + str2->byte_length;
    result->length = str1->length + str2->length;
    
    return result;
}

bs_string* bs_string_append(bs_string* str, const bs_string* append) {
    if (!str) {
        return append ? bs_string_copy(append) : NULL;
    }
    if (!append || append->byte_length == 0) {
        return str;
    }
    
    // 创建一个新的字符串来存储结果
    bs_string* result = bs_string_concat(str, append);
    
    // 释放原始字符串
    bs_string_free(str);
    
    return result;
}

bs_string* bs_string_append_cstr(bs_string* str, const char* append) {
    if (!append) {
        return str;
    }
    
    // 创建临时字符串
    bs_string* append_str = bs_string_new(append);
    if (!append_str) {
        return str;
    }
    
    // 追加并释放临时字符串
    bs_string* result = bs_string_append(str, append_str);
    bs_string_free(append_str);
    
    return result;
}

// 基本的查找操作
bool bs_string_contains_cstr(const bs_string* str, const char* find) {
    if (!str || !find) {
        return false;
    }
    
    bs_string* find_str = bs_string_new(find);
    if (!find_str) {
        return false;
    }
    
    bool result = bs_string_contains(str, find_str);
    bs_string_free(find_str);
    
    return result;
}

bool bs_string_contains(const bs_string* str, const bs_string* find) {
    if (!str || !find) {
        return false;
    }
    
    return bs_string_find(str, find, 0) >= 0;
}

int64_t bs_string_find_cstr(const bs_string* str, const char* find, size_t start_pos) {
    if (!str || !find) {
        return -1;
    }
    
    bs_string* find_str = bs_string_new(find);
    if (!find_str) {
        return -1;
    }
    
    int64_t result = bs_string_find(str, find_str, start_pos);
    bs_string_free(find_str);
    
    return result;
}

int64_t bs_string_find(const bs_string* str, const bs_string* find, size_t start_pos) {
    if (!str || !find || start_pos >= str->length || find->length == 0) {
        return -1;
    }
    
    // 简单的Naive字符串匹配算法
    // 在实际实现中，应该使用更高效的算法如KMP或Boyer-Moore
    
    // 找到起始位置的字节偏移
    size_t start_byte = 0;
    size_t char_count = 0;
    
    while (char_count < start_pos && start_byte < str->byte_length) {
        start_byte += utf8_char_size(str->data + start_byte);
        char_count++;
    }
    
    // 查找子字符串
    for (size_t i = start_byte; i <= str->byte_length - find->byte_length; i++) {
        if (memcmp(str->data + i, find->data, find->byte_length) == 0) {
            // 找到匹配，计算字符位置
            size_t pos = 0;
            for (size_t j = 0; j < i; j += utf8_char_size(str->data + j)) {
                pos++;
            }
            return pos;
        }
    }
    
    return -1;
}

// 其他函数的实现会根据需要添加

/**
 * 计算字符串中单词的数量
 * 单词由空格、制表符或换行符分隔
 */
size_t bs_string_word_count(const bs_string* str) {
    if (!str || !str->data || str->byte_length == 0) {
        return 0;
    }
    
    const char* cstr = (const char*)str->data;
    size_t count = 0;
    bool in_word = false;
    
    for (size_t i = 0; i < str->byte_length; i++) {
        // 判断当前字符是否为空白字符
        bool is_space = isspace(cstr[i]);
        
        // 当从空白字符进入非空白字符时，单词计数增加
        if (!is_space && !in_word) {
            count++;
        }
        
        // 更新状态
        in_word = !is_space;
    }
    
    return count;
}

/**
 * 将字符串首字母大写（每个单词的首字母变为大写，其余为小写）
 */
bs_string* bs_string_capitalize(const bs_string* str) {
    if (!str) {
        return NULL;
    }
    
    // 先创建一个字符串的副本
    bs_string* result = bs_string_copy(str);
    if (!result) {
        return NULL;
    }
    
    bool capitalize_next = true;
    uint8_t* data = result->data;
    
    for (size_t i = 0; i < result->byte_length;) {
        // 只处理ASCII字符，保留非ASCII字符不变
        if ((data[i] & 0x80) == 0) {
            if (isalpha(data[i])) {
                if (capitalize_next) {
                    data[i] = toupper(data[i]);
                    capitalize_next = false;
                } else {
                    data[i] = tolower(data[i]);
                }
            } else if (isspace(data[i]) || ispunct(data[i])) {
                capitalize_next = true;
            }
            i++;
        } else {
            // 跳过非ASCII字符
            i += utf8_char_size(data + i);
            capitalize_next = false;
        }
    }
    
    return result;
}

/**
 * 反转字符串内容
 * 注意：这个函数需要正确处理UTF-8编码
 */
bs_string* bs_string_reverse(const bs_string* str) {
    if (!str) {
        return NULL;
    }
    
    // 特殊情况：空字符串或者只有一个字符
    if (str->byte_length <= 1) {
        return bs_string_copy(str);
    }
    
    // 创建一个新的字符串
    bs_string* result = bs_string_alloc(str->byte_length, str->encoding);
    if (!result) {
        return NULL;
    }
    
    if (str->encoding == BS_ENCODING_UTF8) {
        // UTF-8编码需要特殊处理，按字符反转而不是按字节
        size_t pos = 0;
        size_t char_count = str->length;
        
        // 创建一个数组来存储每个字符的字节偏移量
        size_t* char_offsets = (size_t*)malloc((char_count + 1) * sizeof(size_t));
        if (!char_offsets) {
            bs_string_free(result);
            return NULL;
        }
        
        // 计算每个字符的字节偏移量
        char_offsets[0] = 0;
        size_t char_index = 1;
        for (size_t i = 0; i < str->byte_length && char_index <= char_count;) {
            size_t char_size = utf8_char_size(str->data + i);
            i += char_size;
            char_offsets[char_index] = i;
            char_index++;
        }
        
        // 反向复制每个字符
        for (size_t i = char_count; i > 0; i--) {
            size_t char_start = char_offsets[i - 1];
            size_t char_size = (i < char_count) ? (char_offsets[i] - char_offsets[i - 1]) : (str->byte_length - char_offsets[i - 1]);
            
            memcpy(result->data + pos, str->data + char_start, char_size);
            pos += char_size;
        }
        
        free(char_offsets);
    } else {
        // 对于ASCII或其他单字节编码，可以简单地按字节反向复制
        for (size_t i = 0; i < str->byte_length; i++) {
            result->data[i] = str->data[str->byte_length - 1 - i];
        }
    }
    
    result->length = str->length;
    result->byte_length = str->byte_length;
    result->data[result->byte_length] = '\0';
    
    return result;
}

#endif // BETTER_STRING_IMPLEMENTATION

// C++风格的API (C语言包装)
#ifdef __cplusplus
} // 结束extern "C"
#endif

#ifdef BETTER_STRING_CPP_STYLE

#include <assert.h>

// C++风格的命名空间和宏
#define BS_NAMESPACE_BEGIN typedef struct { } bs_ns; enum : int { bs_priv_
#define BS_NAMESPACE_END };

// 字符串类型定义
typedef bs_string* BsString;

BS_NAMESPACE_BEGIN

// 构造函数
#define BsNew(str) bs_string_new(str)
#define BsNewLen(str, len) bs_string_new_len(str, len)
#define BsNewUtf16(str) bs_string_new_utf16(str)
#define BsNewUtf32(str) bs_string_new_utf32(str)
#define BsCopy(str) bs_string_copy(str)

// 析构函数
#define BsFree(str) bs_string_free(str)

// 属性访问器
#define BsLength(str) bs_string_length(str)
#define BsByteLength(str) bs_string_byte_length(str)
#define BsEncoding(str) bs_string_encoding(str)
#define BsCStr(str) bs_string_c_str(str)
#define BsIsEmpty(str) (bs_string_length(str) == 0)

// 操作方法
#define BsSubstring(str, start, length) bs_string_substring(str, start, length)
#define BsConcat(str1, str2) bs_string_concat(str1, str2)
#define BsAppend(str, append) bs_string_append(str, append)
#define BsAppendCStr(str, append) bs_string_append_cstr(str, append)
#define BsContains(str, find) bs_string_contains(str, find)
#define BsContainsCStr(str, find) bs_string_contains_cstr(str, find)
#define BsFind(str, find, start_pos) bs_string_find(str, find, start_pos)
#define BsFindCStr(str, find, start_pos) bs_string_find_cstr(str, find, start_pos)
#define BsToUpper(str) bs_string_to_upper(str)
#define BsToLower(str) bs_string_to_lower(str)
#define BsCapitalize(str) bs_string_capitalize(str)
#define BsReverse(str) bs_string_reverse(str)
#define BsEquals(str1, str2) bs_string_equals(str1, str2)
#define BsEqualsIgnoreCase(str1, str2) bs_string_equals_ignore_case(str1, str2)
#define BsStartsWith(str, prefix) bs_string_starts_with(str, prefix)
#define BsEndsWith(str, suffix) bs_string_ends_with(str, suffix)
#define BsTrim(str) bs_string_trim(str)
#define BsTrimStart(str) bs_string_trim_start(str)
#define BsTrimEnd(str) bs_string_trim_end(str)
#define BsSplit(str, delimiter) bs_string_split(str, delimiter)
#define BsJoin(array, delimiter) bs_string_join(array, delimiter)
#define BsFormat(...) bs_string_format(__VA_ARGS__)
#define BsWordCount(str) bs_string_word_count(str)

// 链式操作的宏
#define BsChain_Begin(str) do { BsString _bs_temp = (str);
#define BsChain_End() _bs_temp; } while(0)
#define BsChain_Append(append) _bs_temp = BsAppendCStr(_bs_temp, append)
#define BsChain_ToUpper() _bs_temp = BsToUpper(_bs_temp)
#define BsChain_ToLower() _bs_temp = BsToLower(_bs_temp)
#define BsChain_Trim() _bs_temp = BsTrim(_bs_temp)
#define BsChain_Capitalize() _bs_temp = BsCapitalize(_bs_temp)
#define BsChain_Reverse() _bs_temp = BsReverse(_bs_temp)

BS_NAMESPACE_END

#endif // BETTER_STRING_CPP_STYLE

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // BETTER_STRING_SINGLE_H 