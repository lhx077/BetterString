#ifndef BETTER_STRING_CORE_H
#define BETTER_STRING_CORE_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// 确保使用UTF-8编码处理字符串
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// 初始化UTF-8编码环境
inline void bs_init_utf8_environment() {
#ifdef _WIN32
    // 设置控制台代码页为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    // 设置C语言本地化为UTF-8
    setlocale(LC_ALL, ".UTF-8");
}

// 小字符串优化 (SSO) 的缓冲区大小
#define BS_SSO_BUFFER_SIZE 16

// 字符串不存在的标记
#define BS_NPOS ((size_t)-1)

// 字符串存储结构
typedef struct {
    union {
        // 小字符串直接存储
        struct {
            char buffer[BS_SSO_BUFFER_SIZE];
            uint8_t size;
            uint8_t is_small;
        } small;
        
        // 大字符串使用动态分配
        struct {
            char* data;
            size_t size;
            size_t capacity;
            uint8_t is_small;
        } large;
    } storage;
} bs_string_t;

// 初始化一个空字符串
inline void bs_init(bs_string_t* str) {
    str->storage.small.is_small = 1;
    str->storage.small.size = 0;
    str->storage.small.buffer[0] = '\0';
}

// 从C风格字符串初始化
inline void bs_init_from_cstr(bs_string_t* str, const char* cstr) {
    if (!cstr) {
        bs_init(str);
        return;
    }
    
    size_t len = strlen(cstr);
    if (len < BS_SSO_BUFFER_SIZE) {
        str->storage.small.is_small = 1;
        str->storage.small.size = (uint8_t)len;
        if (len > 0) {
            memcpy(str->storage.small.buffer, cstr, len);
        }
        str->storage.small.buffer[len] = '\0';
    } else {
        str->storage.large.is_small = 0;
        str->storage.large.size = len;
        str->storage.large.capacity = len + 1;
        str->storage.large.data = (char*)malloc(str->storage.large.capacity);
        if (len > 0) {
            memcpy(str->storage.large.data, cstr, len);
        }
        str->storage.large.data[len] = '\0';
    }
}

// 从另一个bs_string_t复制
inline void bs_init_copy(bs_string_t* dest, const bs_string_t* src) {
    if (src->storage.small.is_small) {
        memcpy(dest, src, sizeof(bs_string_t));
    } else {
        dest->storage.large.is_small = 0;
        dest->storage.large.size = src->storage.large.size;
        dest->storage.large.capacity = src->storage.large.size + 1;
        dest->storage.large.data = (char*)malloc(dest->storage.large.capacity);
        memcpy(dest->storage.large.data, src->storage.large.data, src->storage.large.size + 1);
    }
}

// 释放字符串资源
inline void bs_free(bs_string_t* str) {
    if (!str->storage.small.is_small) {
        free(str->storage.large.data);
    }
}

// 获取字符串长度
inline size_t bs_length(const bs_string_t* str) {
    return str->storage.small.is_small ? str->storage.small.size : str->storage.large.size;
}

// 获取C风格字符串
inline const char* bs_c_str(const bs_string_t* str) {
    return str->storage.small.is_small ? str->storage.small.buffer : str->storage.large.data;
}

// 获取字符串容量
inline size_t bs_capacity(const bs_string_t* str) {
    return str->storage.small.is_small ? BS_SSO_BUFFER_SIZE - 1 : str->storage.large.capacity - 1;
}

// 确保容量足够
inline void bs_ensure_capacity(bs_string_t* str, size_t capacity) {
    if (str->storage.small.is_small) {
        if (capacity < BS_SSO_BUFFER_SIZE) {
            return; // 仍然可以使用小字符串优化
        }
        
        // 从小字符串转换为大字符串
        char temp[BS_SSO_BUFFER_SIZE];
        size_t tempSize = str->storage.small.size;
        memcpy(temp, str->storage.small.buffer, tempSize + 1);
        
        str->storage.large.is_small = 0;
        str->storage.large.size = tempSize;
        str->storage.large.capacity = capacity + 1;
        str->storage.large.data = (char*)malloc(str->storage.large.capacity);
        memcpy(str->storage.large.data, temp, tempSize + 1);
    } else {
        if (capacity <= str->storage.large.capacity - 1) {
            return; // 当前容量已足够
        }
        
        // 扩展大字符串容量
        size_t newCapacity = capacity + 1;
        if (newCapacity < str->storage.large.capacity * 2) {
            newCapacity = str->storage.large.capacity * 2;
        }
        char* newData = (char*)malloc(newCapacity);
        memcpy(newData, str->storage.large.data, str->storage.large.size + 1);
        free(str->storage.large.data);
        
        str->storage.large.data = newData;
        str->storage.large.capacity = newCapacity;
    }
}

// 清空字符串
inline void bs_clear(bs_string_t* str) {
    if (!str->storage.small.is_small) {
        free(str->storage.large.data);
    }
    bs_init(str);
}

// 获取指定位置的字符
inline char bs_char_at(const bs_string_t* str, size_t index) {
    if (index >= bs_length(str)) {
        return '\0';
    }
    return str->storage.small.is_small ? str->storage.small.buffer[index] : str->storage.large.data[index];
}

// 设置指定位置的字符
inline void bs_set_char_at(bs_string_t* str, size_t index, char c) {
    if (index >= bs_length(str)) {
        return;
    }
    if (str->storage.small.is_small) {
        str->storage.small.buffer[index] = c;
    } else {
        str->storage.large.data[index] = c;
    }
}

// 连接两个字符串
inline void bs_concat(bs_string_t* result, const bs_string_t* str1, const bs_string_t* str2) {
    size_t str1Len = bs_length(str1);
    size_t str2Len = bs_length(str2);
    size_t totalLen = str1Len + str2Len;
    
    bs_init(result);
    bs_ensure_capacity(result, totalLen);
    
    const char* str1Ptr = bs_c_str(str1);
    const char* str2Ptr = bs_c_str(str2);
    
    if (result->storage.small.is_small) {
        if (str1Len > 0) {
            memcpy(result->storage.small.buffer, str1Ptr, str1Len);
        }
        if (str2Len > 0) {
            memcpy(result->storage.small.buffer + str1Len, str2Ptr, str2Len);
        }
        result->storage.small.buffer[totalLen] = '\0';
        result->storage.small.size = (uint8_t)totalLen;
    } else {
        if (str1Len > 0) {
            memcpy(result->storage.large.data, str1Ptr, str1Len);
        }
        if (str2Len > 0) {
            memcpy(result->storage.large.data + str1Len, str2Ptr, str2Len);
        }
        result->storage.large.data[totalLen] = '\0';
        result->storage.large.size = totalLen;
    }
}

// 追加字符串
inline void bs_append(bs_string_t* dest, const bs_string_t* src) {
    size_t destLen = bs_length(dest);
    size_t srcLen = bs_length(src);
    size_t totalLen = destLen + srcLen;
    
    bs_ensure_capacity(dest, totalLen);
    
    const char* srcStr = bs_c_str(src);
    
    if (dest->storage.small.is_small) {
        if (srcLen > 0) {
            memcpy(dest->storage.small.buffer + destLen, srcStr, srcLen);
        }
        dest->storage.small.buffer[totalLen] = '\0';
        dest->storage.small.size = (uint8_t)totalLen;
    } else {
        if (srcLen > 0) {
            memcpy(dest->storage.large.data + destLen, srcStr, srcLen);
        }
        dest->storage.large.data[totalLen] = '\0';
        dest->storage.large.size = totalLen;
    }
}

// 比较两个字符串
inline int bs_compare(const bs_string_t* str1, const bs_string_t* str2) {
    return strcmp(bs_c_str(str1), bs_c_str(str2));
}

// 查找子字符串
inline size_t bs_index_of(const bs_string_t* str, const bs_string_t* substr, size_t startPos) {
    size_t strLen = bs_length(str);
    size_t subLen = bs_length(substr);
    
    if (subLen == 0 || startPos >= strLen) {
        return BS_NPOS;
    }
    
    const char* haystack = bs_c_str(str) + startPos;
    const char* needle = bs_c_str(substr);
    const char* result = strstr(haystack, needle);
    
    if (!result) {
        return BS_NPOS;
    }
    
    return result - bs_c_str(str);
}

// 获取子字符串
inline void bs_substring(bs_string_t* dest, const bs_string_t* src, size_t startPos, size_t count) {
    size_t srcLen = bs_length(src);
    
    if (startPos >= srcLen) {
        bs_init(dest);
        return;
    }
    
    size_t remainingChars = srcLen - startPos;
    size_t actualCount = (count == BS_NPOS || count > remainingChars) ? remainingChars : count;
    
    const char* srcStr = bs_c_str(src);
    
    if (actualCount < BS_SSO_BUFFER_SIZE) {
        dest->storage.small.is_small = 1;
        dest->storage.small.size = (uint8_t)actualCount;
        if (actualCount > 0) {
            memcpy(dest->storage.small.buffer, srcStr + startPos, actualCount);
        }
        dest->storage.small.buffer[actualCount] = '\0';
    } else {
        dest->storage.large.is_small = 0;
        dest->storage.large.size = actualCount;
        dest->storage.large.capacity = actualCount + 1;
        dest->storage.large.data = (char*)malloc(dest->storage.large.capacity);
        if (actualCount > 0) {
            memcpy(dest->storage.large.data, srcStr + startPos, actualCount);
        }
        dest->storage.large.data[actualCount] = '\0';
    }
}

// 转换为大写
inline void bs_to_upper(bs_string_t* dest, const bs_string_t* src) {
    size_t len = bs_length(src);
    
    if (len < BS_SSO_BUFFER_SIZE) {
        dest->storage.small.is_small = 1;
        dest->storage.small.size = (uint8_t)len;
        
        for (size_t i = 0; i < len; ++i) {
            char c = bs_char_at(src, i);
            dest->storage.small.buffer[i] = (c >= 'a' && c <= 'z') ? c - 32 : c;
        }
        
        dest->storage.small.buffer[len] = '\0';
    } else {
        dest->storage.large.is_small = 0;
        dest->storage.large.size = len;
        dest->storage.large.capacity = len + 1;
        dest->storage.large.data = (char*)malloc(dest->storage.large.capacity);
        
        for (size_t i = 0; i < len; ++i) {
            char c = bs_char_at(src, i);
            dest->storage.large.data[i] = (c >= 'a' && c <= 'z') ? c - 32 : c;
        }
        
        dest->storage.large.data[len] = '\0';
    }
}

// 转换为小写
inline void bs_to_lower(bs_string_t* dest, const bs_string_t* src) {
    size_t len = bs_length(src);
    
    if (len < BS_SSO_BUFFER_SIZE) {
        dest->storage.small.is_small = 1;
        dest->storage.small.size = (uint8_t)len;
        
        for (size_t i = 0; i < len; ++i) {
            char c = bs_char_at(src, i);
            dest->storage.small.buffer[i] = (c >= 'A' && c <= 'Z') ? c + 32 : c;
        }
        
        dest->storage.small.buffer[len] = '\0';
    } else {
        dest->storage.large.is_small = 0;
        dest->storage.large.size = len;
        dest->storage.large.capacity = len + 1;
        dest->storage.large.data = (char*)malloc(dest->storage.large.capacity);
        
        for (size_t i = 0; i < len; ++i) {
            char c = bs_char_at(src, i);
            dest->storage.large.data[i] = (c >= 'A' && c <= 'Z') ? c + 32 : c;
        }
        
        dest->storage.large.data[len] = '\0';
    }
}

// 检查字符串是否以指定前缀开头
inline int bs_starts_with(const bs_string_t* str, const bs_string_t* prefix) {
    size_t strLen = bs_length(str);
    size_t prefixLen = bs_length(prefix);
    
    if (prefixLen > strLen) {
        return 0; // 前缀比字符串长，不可能匹配
    }
    
    const char* strPtr = bs_c_str(str);
    const char* prefixPtr = bs_c_str(prefix);
    
    return strncmp(strPtr, prefixPtr, prefixLen) == 0 ? 1 : 0;
}

// 检查字符串是否以指定后缀结尾
inline int bs_ends_with(const bs_string_t* str, const bs_string_t* suffix) {
    size_t strLen = bs_length(str);
    size_t suffixLen = bs_length(suffix);
    
    if (suffixLen > strLen) {
        return 0; // 后缀比字符串长，不可能匹配
    }
    
    const char* strPtr = bs_c_str(str);
    const char* suffixPtr = bs_c_str(suffix);
    
    return strcmp(strPtr + (strLen - suffixLen), suffixPtr) == 0 ? 1 : 0;
}

#ifdef __cplusplus
}
#endif

#endif // BETTER_STRING_CORE_H