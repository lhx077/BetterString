# BetterString 使用指南 | BetterString Usage Guide

## 简介 | Introduction

BetterString 是一个高性能的字符串处理库，提供了丰富的字符串操作功能，支持多种编码和各种现代字符集。本库同时提供C和C++两种实现，保持API风格统一。

BetterString is a high-performance string processing library that provides rich string manipulation functions, supports various encodings and modern character sets. The library offers both C and C++ implementations with a unified API style.

## 基本用法 | Basic Usage

### C版本 | C Version

```c
#include <better_string.h>
#include <stdio.h>

int main() {
    // 创建字符串 | Create a string
    bs_string* str = bs_string_new("Hello, World!");
    
    // 基本操作 | Basic operations
    printf("Length: %zu\n", bs_string_length(str));
    printf("Content: %s\n", bs_string_c_str(str));
    
    // 字符串转换 | String conversion
    bs_string* upper = bs_string_to_upper(str);
    printf("Upper: %s\n", bs_string_c_str(upper));
    
    // 字符串查找 | String search
    bool contains = bs_string_contains_cstr(str, "World");
    printf("Contains 'World': %d\n", contains);
    
    // 字符串修改 | String modification
    bs_string* new_str = bs_string_append_cstr(str, " Welcome!");
    printf("New string: %s\n", bs_string_c_str(new_str));
    
    // 清理资源 | Cleanup
    bs_string_free(str);
    bs_string_free(upper);
    bs_string_free(new_str);
    
    return 0;
}
```

### C++版本 | C++ Version

```cpp
#include <better_string.hpp>
#include <iostream>

int main() {
    // 创建字符串 | Create a string
    bs::String str("Hello, World!");
    
    // 基本操作 | Basic operations
    std::cout << "Length: " << str.length() << std::endl;
    std::cout << "Content: " << str << std::endl;
    
    // 字符串转换 | String conversion
    bs::String upper = str.toUpperCopy();
    std::cout << "Upper: " << upper << std::endl;
    
    // 字符串查找 | String search
    bool contains = str.contains("World");
    std::cout << "Contains 'World': " << contains << std::endl;
    
    // 字符串修改 | String modification
    str.append(" Welcome!");
    std::cout << "New string: " << str << std::endl;
    
    return 0;
}
```

## 编码支持 | Encoding Support

BetterString 支持多种字符编码：

BetterString supports multiple character encodings:

- UTF-8 (默认 | default)
- UTF-16
- UTF-32
- ASCII

### 编码示例 | Encoding Example

```c
// C版本 | C version
uint16_t utf16_data[] = {0x4E2D, 0x6587, 0}; // "中文" in UTF-16
bs_string* utf16_str = bs_string_new_utf16(utf16_data);

uint32_t utf32_data[] = {0x4E2D, 0x6587, 0}; // "中文" in UTF-32
bs_string* utf32_str = bs_string_new_utf32(utf32_data);
```

```cpp
// C++版本 | C++ version
std::u16string u16str = u"中文";
bs::String str = bs::String(u16str.c_str());

// 转换到UTF-16 | Convert to UTF-16
std::u16string utf16 = str.toUtf16();
```

## 国际化支持 | Internationalization Support

BetterString 支持包括但不限于以下语言的字符：

BetterString supports characters from languages including but not limited to:

- 英文 (Latin) | English
- 中文 (Chinese)
- 日文 (Japanese)
- 韩文 (Korean)
- 阿拉伯文 (Arabic)
- 俄文 (Russian)
- 表情符号 (Emoji)

### 示例 | Example

```c
// C版本 | C version
bs_string* multilingual = bs_string_new("English 中文 日本語 한국어 العربية Русский 😊");
printf("%s\n", bs_string_c_str(multilingual));
```

```cpp
// C++版本 | C++ version
bs::String multilingual("English 中文 日本語 한국어 العربية Русский 😊");
std::cout << multilingual << std::endl;
```

## 主要功能 | Main Features

### 字符串创建 | String Creation

```c
// C版本 | C version
bs_string* str1 = bs_string_new("Hello");
bs_string* str2 = bs_string_new_len("Hello World", 5); // 只取前5个字符 | Take only first 5 chars
```

```cpp
// C++版本 | C++ version
bs::String str1("Hello");
bs::String str2(std::string("Hello"));
```

### 字符串转换 | String Conversion

```c
// C版本 | C version
bs_string* upper = bs_string_to_upper(str);
bs_string* lower = bs_string_to_lower(str);
```

```cpp
// C++版本 | C++ version
bs::String upper = str.toUpperCopy();
str.toLower(); // 直接修改 | Modify directly
```

### 字符串查找 | String Search

```c
// C版本 | C version
int64_t pos = bs_string_find_cstr(str, "world", 0);
bool contains = bs_string_contains_cstr(str, "hello");
bool starts = bs_string_starts_with(str, prefix);
bool ends = bs_string_ends_with(str, suffix);
```

```cpp
// C++版本 | C++ version
size_t pos = str.find("world");
bool contains = str.contains("hello");
bool starts = str.startsWith(prefix);
bool ends = str.endsWith(suffix);
```

### 字符串分割与连接 | String Split and Join

```c
// C版本 | C version
bs_string_array* parts = bs_string_split(str, ",");
bs_string* joined = bs_string_join(parts, "-");
```

```cpp
// C++版本 | C++ version
std::vector<bs::String> parts = str.split(",");
bs::String joined = bs::String::join(parts, "-");
```

## API 参考 | API Reference

更详细的API文档请参见以下文件：

For more detailed API documentation, please refer to the following files:

- [C API 参考](./c_api_reference.md) | [C API Reference](./c_api_reference.md)
- [C++ API 参考](./cpp_api_reference.md) | [C++ API Reference](./cpp_api_reference.md)

## 性能考虑 | Performance Considerations

BetterString 在设计时注重高性能，采用了以下优化策略：

BetterString is designed with high performance in mind, using the following optimization strategies:

1. 延迟计算 | Lazy evaluation
2. 内存池 | Memory pooling
3. 小字符串优化 | Small string optimization
4. 字符串视图 | String views
5. 高效的字符串匹配算法 | Efficient string matching algorithms

## 内存管理 | Memory Management

### C版本 | C Version

C版本的库需要手动管理内存：

The C version of the library requires manual memory management:

```c
bs_string* str = bs_string_new("Hello");
// 使用字符串... | Use the string...
bs_string_free(str); // 必须释放 | Must free
```

### C++版本 | C++ Version

C++版本使用RAII，自动管理内存：

The C++ version uses RAII for automatic memory management:

```cpp
{
    bs::String str("Hello");
    // 使用字符串... | Use the string...
} // 自动析构 | Automatic destruction
``` 