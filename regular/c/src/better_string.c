/**
 * better_string.c - 高性能字符串库的C语言实现
 * 
 * 这个库提供了一个高性能、功能丰富的字符串处理库，
 * 支持Unicode和各种现代字符串操作。
 */

#include "../include/better_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

// 内部工具函数
static size_t utf8_strlen(const char* str, size_t byte_length);
static size_t utf8_char_size(const uint8_t* str);
static uint32_t utf8_decode_char(const uint8_t* str);
static size_t utf8_encode_char(uint32_t ch, uint8_t* buffer);
static bs_string* bs_string_alloc(size_t byte_length, bs_encoding_t encoding);

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

// 基本字符串操作实现
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

// 查找和比较操作
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

// 格式化字符串
bs_string* bs_string_format(const char* format, ...) {
    if (!format) {
        return bs_string_new("");
    }
    
    va_list args;
    va_start(args, format);
    
    // 首先计算需要的缓冲区大小
    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    
    if (len < 0) {
        va_end(args);
        return bs_string_new("");
    }
    
    // 分配内存
    char* buffer = (char*)malloc(len + 1);
    if (!buffer) {
        va_end(args);
        return bs_string_new("");
    }
    
    // 格式化字符串
    vsnprintf(buffer, len + 1, format, args);
    va_end(args);
    
    // 创建字符串对象
    bs_string* result = bs_string_new(buffer);
    free(buffer);
    
    return result;
}

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

// 其他函数实现会在需要时添加... 