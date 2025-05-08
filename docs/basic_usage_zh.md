# BetterString 使用指南

## 简介

BetterString 是一个高性能的字符串处理库，提供了丰富的字符串操作功能，支持多种编码和各种现代字符集。本库同时提供C和C++两种实现，保持API风格统一。

## 基本用法

### C版本

```c
#include <better_string.h>
#include <stdio.h>

int main() {
    // 创建字符串
    bs_string* str = bs_string_new("Hello, World!");
    
    // 基本操作
    printf("Length: %zu\n", bs_string_length(str));
    printf("Content: %s\n", bs_string_c_str(str));
    
    // 字符串转换
    bs_string* upper = bs_string_to_upper(str);
    printf("Upper: %s\n", bs_string_c_str(upper));
    
    // 字符串查找
    bool contains = bs_string_contains_cstr(str, "World");
    printf("Contains 'World': %d\n", contains);
    
    // 字符串修改
    bs_string* new_str = bs_string_append_cstr(str, " Welcome!");
    printf("New string: %s\n", bs_string_c_str(new_str));
    
    // 清理资源
    bs_string_free(str);
    bs_string_free(upper);
    bs_string_free(new_str);
    
    return 0;
}
```

### C++版本

```cpp
#include <better_string.hpp>
#include <iostream>

int main() {
    // 创建字符串
    bs::String str("Hello, World!");
    
    // 基本操作
    std::cout << "Length: " << str.length() << std::endl;
    std::cout << "Content: " << str << std::endl;
    
    // 字符串转换
    bs::String upper = str.toUpperCopy();
    std::cout << "Upper: " << upper << std::endl;
    
    // 字符串查找
    bool contains = str.contains("World");
    std::cout << "Contains 'World': " << contains << std::endl;
    
    // 字符串修改
    str.append(" Welcome!");
    std::cout << "New string: " << str << std::endl;
    
    return 0;
}
```

## 编码支持

BetterString 支持多种字符编码：

- UTF-8 (默认)
- UTF-16
- UTF-32
- ASCII

### 编码示例

```c
// C版本
uint16_t utf16_data[] = {0x4E2D, 0x6587, 0}; // UTF-16编码的"中文"
bs_string* utf16_str = bs_string_new_utf16(utf16_data);

uint32_t utf32_data[] = {0x4E2D, 0x6587, 0}; // UTF-32编码的"中文"
bs_string* utf32_str = bs_string_new_utf32(utf32_data);
```

```cpp
// C++版本
std::u16string u16str = u"中文";
bs::String str = bs::String(u16str.c_str());

// 转换到UTF-16
std::u16string utf16 = str.toUtf16();
```

## 国际化支持

BetterString 支持包括但不限于以下语言的字符：

- 英文 (Latin)
- 中文 (Chinese)
- 日文 (Japanese)
- 韩文 (Korean)
- 阿拉伯文 (Arabic)
- 俄文 (Russian)
- 表情符号 (Emoji)

### 示例

```c
// C版本
bs_string* multilingual = bs_string_new("English 中文 日本語 한국어 العربية Русский 😊");
printf("%s\n", bs_string_c_str(multilingual));
```

```cpp
// C++版本
bs::String multilingual("English 中文 日本語 한국어 العربية Русский 😊");
std::cout << multilingual << std::endl;
```

## 主要功能

### 字符串创建

```c
// C版本
bs_string* str1 = bs_string_new("Hello");
bs_string* str2 = bs_string_new_len("Hello World", 5); // 只取前5个字符
```

```cpp
// C++版本
bs::String str1("Hello");
bs::String str2(std::string("Hello"));
```

### 字符串转换

```c
// C版本
bs_string* upper = bs_string_to_upper(str);
bs_string* lower = bs_string_to_lower(str);
```

```cpp
// C++版本
bs::String upper = str.toUpperCopy();
str.toLower(); // 直接修改
```

### 字符串查找

```c
// C版本
int64_t pos = bs_string_find_cstr(str, "world", 0);
bool contains = bs_string_contains_cstr(str, "hello");
bool starts = bs_string_starts_with(str, prefix);
bool ends = bs_string_ends_with(str, suffix);
```

```cpp
// C++版本
size_t pos = str.find("world");
bool contains = str.contains("hello");
bool starts = str.startsWith(prefix);
bool ends = str.endsWith(suffix);
```

### 字符串分割与连接

```c
// C版本
bs_string_array* parts = bs_string_split(str, ",");
bs_string* joined = bs_string_join(parts, "-");
```

```cpp
// C++版本
std::vector<bs::String> parts = str.split(",");
bs::String joined = bs::String::join(parts, "-");
```

## API 参考

更详细的API文档请参见以下文件：

- [API 参考](./api_reference_zh.md)

## 性能考虑

BetterString 在设计时注重高性能，采用了以下优化策略：

1. 延迟计算
2. 内存池
3. 小字符串优化
4. 字符串视图
5. 高效的字符串匹配算法

## 内存管理

### C版本

C版本的库需要手动管理内存：

```c
bs_string* str = bs_string_new("Hello");
// 使用字符串...
bs_string_free(str); // 必须释放
```

### C++版本

C++版本使用RAII，自动管理内存：

```cpp
{
    bs::String str("Hello");
    // 使用字符串...
} // 自动析构
``` 