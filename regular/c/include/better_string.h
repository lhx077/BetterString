/**
 * better_string.h - 高性能字符串库的C语言头文件
 * 
 * 这个库提供了一个高性能、功能丰富的字符串处理库，
 * 支持Unicode和各种现代字符串操作。
 */

#ifndef BETTER_STRING_H
#define BETTER_STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

/**
 * 构造和析构函数
 */
bs_string* bs_string_new(const char* str);
bs_string* bs_string_new_len(const char* str, size_t length);
bs_string* bs_string_new_utf16(const uint16_t* str);
bs_string* bs_string_new_utf32(const uint32_t* str);
bs_string* bs_string_new_with_encoding(const void* str, size_t byte_length, bs_encoding_t encoding);
bs_string* bs_string_copy(const bs_string* str);
void bs_string_free(bs_string* str);

/**
 * 属性访问
 */
size_t bs_string_length(const bs_string* str);
size_t bs_string_byte_length(const bs_string* str);
bs_encoding_t bs_string_encoding(const bs_string* str);
const char* bs_string_c_str(const bs_string* str);

/**
 * 字符串操作
 */
bs_string* bs_string_substring(const bs_string* str, size_t start, size_t length);
bs_string* bs_string_concat(const bs_string* str1, const bs_string* str2);
bs_string* bs_string_append(bs_string* str, const bs_string* append);
bs_string* bs_string_append_cstr(bs_string* str, const char* append);
bs_string* bs_string_insert(bs_string* str, size_t position, const bs_string* insert);
bs_string* bs_string_delete(bs_string* str, size_t start, size_t length);
bs_string* bs_string_replace(bs_string* str, const bs_string* find, const bs_string* replace);

/**
 * 修饰方法
 */
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

/**
 * 查找和比较
 */
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

/**
 * 分割和连接
 */
bs_string_array* bs_string_split(const bs_string* str, const char* delimiter);
bs_string* bs_string_join(const bs_string_array* array, const char* delimiter);
void bs_string_array_free(bs_string_array* array);

/**
 * 统计功能
 */
/**
 * 计算字符串中单词的数量
 * @param str 源字符串
 * @return 字符串中单词的数量
 */
size_t bs_string_word_count(const bs_string* str);

/**
 * 其他操作
 */
uint32_t bs_string_char_at(const bs_string* str, size_t index);
bs_string* bs_string_char_at_str(const bs_string* str, size_t index);
bs_iterator* bs_string_iterator_new(const bs_string* str);
bool bs_string_iterator_has_next(const bs_iterator* it);
uint32_t bs_string_iterator_next(bs_iterator* it);
void bs_string_iterator_free(bs_iterator* it);

/**
 * 转换方法
 */
bs_string* bs_string_from_int(int64_t value);
bs_string* bs_string_from_float(double value);
int64_t bs_string_to_int(const bs_string* str);
double bs_string_to_float(const bs_string* str);
bool bs_string_try_parse_int(const bs_string* str, int64_t* value);
bool bs_string_try_parse_float(const bs_string* str, double* value);

/**
 * 正则表达式操作
 */
bool bs_string_matches(const bs_string* str, const char* pattern);
bs_string* bs_string_replace_regex(const bs_string* str, const char* pattern, const char* replacement);
bs_string_array* bs_string_regex_split(const bs_string* str, const char* pattern);
bs_string_array* bs_string_regex_find_all(const bs_string* str, const char* pattern);
bs_string* bs_string_format(const char* format, ...);

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
}
#endif

#endif // BETTER_STRING_H 