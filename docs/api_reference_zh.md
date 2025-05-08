# BetterString API 参考文档

本文档提供了BetterString库的详细API参考，包括C和C++版本的所有函数和方法。

## C API 参考

### 基本类型

```c
// 字符串编码类型
typedef enum {
    BS_ENCODING_UTF8,    // UTF-8编码
    BS_ENCODING_UTF16,   // UTF-16编码
    BS_ENCODING_UTF32,   // UTF-32编码
    BS_ENCODING_ASCII    // ASCII编码
} bs_encoding_t;

// 字符串对象类型
typedef struct bs_string {
    uint8_t* data;           // 原始字节数据
    size_t length;           // 字符数量（不是字节长度）
    size_t byte_length;      // 字节长度
    bs_encoding_t encoding;  // 编码类型
    bool is_view;            // 是否为视图（不拥有内存）
} bs_string;

// 字符串数组类型
typedef struct bs_string_array {
    bs_string** items;       // 字符串数组
    size_t length;           // 数组长度
} bs_string_array;

// 迭代器类型
typedef struct bs_iterator {
    const bs_string* str;    // 被迭代的字符串
    size_t position;         // 当前字节位置
    size_t char_position;    // 当前字符位置
} bs_iterator;
```

### 构造与析构函数

| 函数 | 描述 |
|-----|------|
| `bs_string* bs_string_new(const char* str)` | 从C风格字符串创建一个新的字符串对象 |
| `bs_string* bs_string_new_len(const char* str, size_t length)` | 从具有指定长度的C风格字符串创建一个新的字符串对象 |
| `bs_string* bs_string_new_utf16(const uint16_t* str)` | 从UTF-16字符串创建一个新的字符串对象 |
| `bs_string* bs_string_new_utf32(const uint32_t* str)` | 从UTF-32字符串创建一个新的字符串对象 |
| `bs_string* bs_string_new_with_encoding(const void* str, size_t byte_length, bs_encoding_t encoding)` | 从具有指定编码的数据创建一个新的字符串对象 |
| `bs_string* bs_string_copy(const bs_string* str)` | 创建一个字符串的拷贝 |
| `void bs_string_free(bs_string* str)` | 释放字符串对象及其关联的内存 |

### 属性访问函数

| 函数 | 描述 |
|-----|------|
| `size_t bs_string_length(const bs_string* str)` | 获取字符串的字符数量 |
| `size_t bs_string_byte_length(const bs_string* str)` | 获取字符串的字节长度 |
| `bs_encoding_t bs_string_encoding(const bs_string* str)` | 获取字符串的编码类型 |
| `const char* bs_string_c_str(const bs_string* str)` | 获取作为以null结尾的C字符串的表示形式 |

### 基本字符串操作

| 函数 | 描述 |
|-----|------|
| `bs_string* bs_string_substring(const bs_string* str, size_t start, size_t length)` | 提取子字符串 |
| `bs_string* bs_string_concat(const bs_string* str1, const bs_string* str2)` | 连接两个字符串 |
| `bs_string* bs_string_append(bs_string* str, const bs_string* append)` | 将一个字符串附加到另一个字符串 |
| `bs_string* bs_string_append_cstr(bs_string* str, const char* append)` | 将C字符串附加到字符串 |
| `bs_string* bs_string_insert(bs_string* str, size_t position, const bs_string* insert)` | 在指定位置插入字符串 |
| `bs_string* bs_string_delete(bs_string* str, size_t start, size_t length)` | 删除子字符串 |
| `bs_string* bs_string_replace(bs_string* str, const bs_string* find, const bs_string* replace)` | 替换字符串的所有出现 |

### 修饰函数

| 函数 | 描述 |
|-----|------|
| `bs_string* bs_string_trim(const bs_string* str)` | 修剪字符串两端的空白字符 |
| `bs_string* bs_string_trim_start(const bs_string* str)` | 修剪字符串开头的空白字符 |
| `bs_string* bs_string_trim_end(const bs_string* str)` | 修剪字符串结尾的空白字符 |
| `bs_string* bs_string_to_upper(const bs_string* str)` | 将字符串转换为大写 |
| `bs_string* bs_string_to_lower(const bs_string* str)` | 将字符串转换为小写 |
| `bs_string* bs_string_capitalize(const bs_string* str)` | 将每个单词的首字母大写，其余字母小写 |
| `bs_string* bs_string_reverse(const bs_string* str)` | 反转字符串内容 |

### 查找和比较函数

| 函数 | 描述 |
|-----|------|
| `int64_t bs_string_find(const bs_string* str, const bs_string* find, size_t start_pos)` | 查找子字符串，返回位置或-1（未找到） |
| `int64_t bs_string_find_cstr(const bs_string* str, const char* find, size_t start_pos)` | 查找C字符串，返回位置或-1（未找到） |
| `int64_t bs_string_rfind(const bs_string* str, const bs_string* find, size_t start_pos)` | 从末尾开始查找子字符串 |
| `bool bs_string_contains(const bs_string* str, const bs_string* find)` | 检查字符串是否包含子字符串 |
| `bool bs_string_contains_cstr(const bs_string* str, const char* find)` | 检查字符串是否包含C字符串 |
| `bool bs_string_starts_with(const bs_string* str, const bs_string* prefix)` | 检查字符串是否以前缀开头 |
| `bool bs_string_ends_with(const bs_string* str, const bs_string* suffix)` | 检查字符串是否以后缀结尾 |
| `int bs_string_compare(const bs_string* str1, const bs_string* str2)` | 比较两个字符串，返回<0,0,>0 |
| `bool bs_string_equals(const bs_string* str1, const bs_string* str2)` | 检查两个字符串是否相等 |
| `bool bs_string_equals_ignore_case(const bs_string* str1, const bs_string* str2)` | 忽略大小写检查相等性 |

### 分割和连接函数

| 函数 | 描述 |
|-----|------|
| `bs_string_array* bs_string_split(const bs_string* str, const char* delimiter)` | 使用分隔符分割字符串 |
| `bs_string* bs_string_join(const bs_string_array* array, const char* delimiter)` | 使用分隔符连接字符串数组 |
| `void bs_string_array_free(bs_string_array* array)` | 释放字符串数组 |

### 统计函数

| 函数 | 描述 |
|-----|------|
| `size_t bs_string_word_count(const bs_string* str)` | 计算字符串中单词的数量 |

### 字符级操作

| 函数 | 描述 |
|-----|------|
| `uint32_t bs_string_char_at(const bs_string* str, size_t index)` | 获取指定索引处的Unicode字符 |
| `bs_string* bs_string_char_at_str(const bs_string* str, size_t index)` | 获取指定索引处的字符作为字符串 |
| `bs_iterator* bs_string_iterator_new(const bs_string* str)` | 创建一个字符串迭代器 |
| `bool bs_string_iterator_has_next(const bs_iterator* it)` | 检查迭代器是否有更多字符 |
| `uint32_t bs_string_iterator_next(bs_iterator* it)` | 获取下一个字符并前进迭代器 |
| `void bs_string_iterator_free(bs_iterator* it)` | 释放迭代器 |

### 转换函数

| 函数 | 描述 |
|-----|------|
| `bs_string* bs_string_from_int(int64_t value)` | 从整数创建字符串 |
| `bs_string* bs_string_from_float(double value)` | 从浮点数创建字符串 |
| `int64_t bs_string_to_int(const bs_string* str)` | 将字符串转换为整数 |
| `double bs_string_to_float(const bs_string* str)` | 将字符串转换为浮点数 |
| `bool bs_string_try_parse_int(const bs_string* str, int64_t* value)` | 尝试将字符串解析为整数 |
| `bool bs_string_try_parse_float(const bs_string* str, double* value)` | 尝试将字符串解析为浮点数 |

### 正则表达式函数

| 函数 | 描述 |
|-----|------|
| `bool bs_string_matches(const bs_string* str, const char* pattern)` | 检查字符串是否匹配正则表达式模式 |
| `bs_string* bs_string_replace_regex(const bs_string* str, const char* pattern, const char* replacement)` | 使用正则表达式替换匹配 |
| `bs_string_array* bs_string_regex_split(const bs_string* str, const char* pattern)` | 使用正则表达式分割字符串 |
| `bs_string_array* bs_string_regex_find_all(const bs_string* str, const char* pattern)` | 查找正则表达式的所有匹配项 |
| `bs_string* bs_string_format(const char* format, ...)` | 格式化字符串（类似于printf） |

### C++ 风格 API (C 语言包装)

使用 `BETTER_STRING_CPP_STYLE` 定义可启用以下宏：

#### 构造与析构

| C++ 风格宏 | C 函数 |
|-----------|---------|
| `BsNew(str)` | `bs_string_new(str)` |
| `BsNewLen(str, len)` | `bs_string_new_len(str, len)` |
| `BsNewUtf16(str)` | `bs_string_new_utf16(str)` |
| `BsNewUtf32(str)` | `bs_string_new_utf32(str)` |
| `BsCopy(str)` | `bs_string_copy(str)` |
| `BsFree(str)` | `bs_string_free(str)` |

#### 属性访问

| C++ 风格宏 | C 函数 |
|-----------|---------|
| `BsLength(str)` | `bs_string_length(str)` |
| `BsByteLength(str)` | `bs_string_byte_length(str)` |
| `BsEncoding(str)` | `bs_string_encoding(str)` |
| `BsCStr(str)` | `bs_string_c_str(str)` |
| `BsIsEmpty(str)` | `bs_string_length(str) == 0` |

#### 操作方法

| C++ 风格宏 | C 函数 |
|-----------|---------|
| `BsSubstring(str, start, length)` | `bs_string_substring(str, start, length)` |
| `BsConcat(str1, str2)` | `bs_string_concat(str1, str2)` |
| `BsAppend(str, append)` | `bs_string_append(str, append)` |
| `BsAppendCStr(str, append)` | `bs_string_append_cstr(str, append)` |
| `BsContains(str, find)` | `bs_string_contains(str, find)` |
| `BsContainsCStr(str, find)` | `bs_string_contains_cstr(str, find)` |
| `BsFind(str, find, start_pos)` | `bs_string_find(str, find, start_pos)` |
| `BsFindCStr(str, find, start_pos)` | `bs_string_find_cstr(str, find, start_pos)` |
| `BsToUpper(str)` | `bs_string_to_upper(str)` |
| `BsToLower(str)` | `bs_string_to_lower(str)` |
| `BsCapitalize(str)` | `bs_string_capitalize(str)` |
| `BsReverse(str)` | `bs_string_reverse(str)` |
| `BsEquals(str1, str2)` | `bs_string_equals(str1, str2)` |
| `BsEqualsIgnoreCase(str1, str2)` | `bs_string_equals_ignore_case(str1, str2)` |
| `BsStartsWith(str, prefix)` | `bs_string_starts_with(str, prefix)` |
| `BsEndsWith(str, suffix)` | `bs_string_ends_with(str, suffix)` |
| `BsTrim(str)` | `bs_string_trim(str)` |
| `BsTrimStart(str)` | `bs_string_trim_start(str)` |
| `BsTrimEnd(str)` | `bs_string_trim_end(str)` |
| `BsSplit(str, delimiter)` | `bs_string_split(str, delimiter)` |
| `BsJoin(array, delimiter)` | `bs_string_join(array, delimiter)` |
| `BsFormat(...)` | `bs_string_format(...)` |
| `BsWordCount(str)` | `bs_string_word_count(str)` |

#### 链式操作宏

```c
BsChain_Begin(str);
BsChain_Append(append);
BsChain_ToUpper();
BsChain_ToLower();
BsChain_Trim();
BsChain_Capitalize();
BsChain_Reverse();
BsChain_End();
```

## C++ API 参考

C++ API 提供了一个面向对象的接口，使用更自然和直观的方式操作字符串。

### 构造函数

```cpp
String();                                    // 创建一个空字符串
String(const char* str);                     // 从C风格字符串创建
String(const std::string& str);              // 从std::string创建
String(const String& other);                 // 拷贝构造函数
String(String&& other) noexcept;             // 移动构造函数
explicit String(bs_string* str, bool own);   // 从C API字符串构造（高级用法）
```

### 析构函数

```cpp
~String();                                   // 释放字符串资源
```

### 赋值操作符

```cpp
String& operator=(const String& other);      // 从另一个String赋值
String& operator=(String&& other) noexcept;  // 移动赋值
String& operator=(const char* str);          // 从C字符串赋值
String& operator=(const std::string& str);   // 从std::string赋值
```

### 基本属性

```cpp
size_t length() const;                       // 获取字符数量
size_t byteLength() const;                   // 获取字节长度
bool isEmpty() const;                        // 检查是否为空
size_t wordCount() const;                    // 计算单词数量
```

### 转换方法

```cpp
const char* c_str() const;                   // 获取C风格字符串
std::string toString() const;                // 转换为std::string
std::u16string toUtf16() const;              // 转换为UTF-16
std::u32string toUtf32() const;              // 转换为UTF-32
```

### 字符串操作

```cpp
String substring(size_t start, size_t length = std::string::npos) const;  // 提取子字符串
String& append(const String& str);           // 附加字符串
String& append(const char* str);             // 附加C字符串
String& append(const std::string& str);      // 附加std::string
String operator+(const String& other) const; // 连接操作符
String operator+(const char* str) const;     // 与C字符串连接
String operator+(const std::string& str) const; // 与std::string连接
```

### 修改操作

```cpp
String& insert(size_t position, const String& str);  // 插入字符串
String& erase(size_t position, size_t length = std::string::npos);  // 删除部分
String& replace(const String& from, const String& to);  // 替换字符串
String& replaceAll(const String& from, const String& to);  // 替换所有匹配
String& capitalize();                        // 将每个单词首字母大写
String& reverse();                           // 反转字符串内容
```

### 比较操作

```cpp
bool operator==(const String& other) const;  // 相等比较
bool operator!=(const String& other) const;  // 不等比较
bool operator<(const String& other) const;   // 小于
bool operator<=(const String& other) const;  // 小于等于
bool operator>(const String& other) const;   // 大于
bool operator>=(const String& other) const;  // 大于等于
int compare(const String& other) const;      // 比较两个字符串
bool equals(const String& other, bool ignoreCase = false) const;  // 检查相等性
```

### 查找方法

```cpp
size_t find(const String& str, size_t startPos = 0) const;  // 查找子字符串
size_t find(const char* str, size_t startPos = 0) const;    // 查找C字符串
size_t rfind(const String& str, size_t startPos = std::string::npos) const;  // 反向查找
bool contains(const String& str) const;      // 检查是否包含
bool contains(const char* str) const;        // 检查是否包含C字符串
bool startsWith(const String& prefix) const; // 检查前缀
bool endsWith(const String& suffix) const;   // 检查后缀
```

### 修饰方法

```cpp
String& trim();                              // 修剪两端空白
String& trimLeft();                          // 修剪左侧空白
String& trimRight();                         // 修剪右侧空白
String& toLower();                           // 转为小写
String& toUpper();                           // 转为大写

// 非修改的副本版本
String toLowerCopy() const;                  // 获取小写副本
String toUpperCopy() const;                  // 获取大写副本
String capitalizeCopy() const;               // 获取首字母大写副本
String reverseCopy() const;                  // 获取反转副本
String trimCopy() const;                     // 获取修剪副本
```

### 分割与连接

```cpp
std::vector<String> split(const String& delimiter) const;  // 使用分隔符分割
std::vector<String> split(const char* delimiter) const;    // 使用C字符串分割
static String join(const std::vector<String>& strings, const String& delimiter);  // 连接字符串
```

### 字符级操作

```cpp
char32_t charAt(size_t index) const;         // 获取指定位置的字符
String charAtAsString(size_t index) const;   // 获取字符作为字符串
```

### 格式化与转换

```cpp
static String format(const char* format, ...); // 格式化字符串
static String fromInt(int64_t value);        // 从整数创建
static String fromFloat(double value);       // 从浮点数创建
int64_t toInt() const;                       // 转换为整数
double toFloat() const;                      // 转换为浮点数
bool tryParseInt(int64_t& value) const;      // 尝试解析整数
bool tryParseFloat(double& value) const;     // 尝试解析浮点数
```

### 正则表达式操作

```cpp
bool matches(const String& pattern) const;   // 匹配正则表达式
String& replaceRegex(const String& pattern, const String& replacement);  // 使用正则表达式替换
std::vector<String> regexSplit(const String& pattern) const;  // 使用正则表达式分割
std::vector<String> regexFindAll(const String& pattern) const;  // 查找所有匹配
```

### 高级用法

```cpp
bs_string* getCString() const;               // 获取底层C字符串（高级用法）
```

### 非成员函数

```cpp
String operator+(const char* lhs, const String& rhs);  // C字符串与String连接
String operator+(const std::string& lhs, const String& rhs);  // std::string与String连接
std::ostream& operator<<(std::ostream& os, const String& str);  // 流输出
std::istream& operator>>(std::istream& is, String& str);  // 流输入
``` 