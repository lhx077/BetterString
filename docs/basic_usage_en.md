# BetterString Usage Guide

## Introduction

BetterString is a high-performance string processing library that provides rich string manipulation functions, supports various encodings and modern character sets. The library offers both C and C++ implementations with a unified API style.

## Basic Usage

### C Version

```c
#include <better_string.h>
#include <stdio.h>

int main() {
    // Create a string
    bs_string* str = bs_string_new("Hello, World!");
    
    // Basic operations
    printf("Length: %zu\n", bs_string_length(str));
    printf("Content: %s\n", bs_string_c_str(str));
    
    // String conversion
    bs_string* upper = bs_string_to_upper(str);
    printf("Upper: %s\n", bs_string_c_str(upper));
    
    // String search
    bool contains = bs_string_contains_cstr(str, "World");
    printf("Contains 'World': %d\n", contains);
    
    // String modification
    bs_string* new_str = bs_string_append_cstr(str, " Welcome!");
    printf("New string: %s\n", bs_string_c_str(new_str));
    
    // Cleanup
    bs_string_free(str);
    bs_string_free(upper);
    bs_string_free(new_str);
    
    return 0;
}
```

### C++ Version

```cpp
#include <better_string.hpp>
#include <iostream>

int main() {
    // Create a string
    bs::String str("Hello, World!");
    
    // Basic operations
    std::cout << "Length: " << str.length() << std::endl;
    std::cout << "Content: " << str << std::endl;
    
    // String conversion
    bs::String upper = str.toUpperCopy();
    std::cout << "Upper: " << upper << std::endl;
    
    // String search
    bool contains = str.contains("World");
    std::cout << "Contains 'World': " << contains << std::endl;
    
    // String modification
    str.append(" Welcome!");
    std::cout << "New string: " << str << std::endl;
    
    return 0;
}
```

## Encoding Support

BetterString supports multiple character encodings:

- UTF-8 (default)
- UTF-16
- UTF-32
- ASCII

### Encoding Example

```c
// C version
uint16_t utf16_data[] = {0x4E2D, 0x6587, 0}; // "中文" in UTF-16
bs_string* utf16_str = bs_string_new_utf16(utf16_data);

uint32_t utf32_data[] = {0x4E2D, 0x6587, 0}; // "中文" in UTF-32
bs_string* utf32_str = bs_string_new_utf32(utf32_data);
```

```cpp
// C++ version
std::u16string u16str = u"中文";
bs::String str = bs::String(u16str.c_str());

// Convert to UTF-16
std::u16string utf16 = str.toUtf16();
```

## Internationalization Support

BetterString supports characters from languages including but not limited to:

- English (Latin)
- Chinese
- Japanese
- Korean
- Arabic
- Russian
- Emoji

### Example

```c
// C version
bs_string* multilingual = bs_string_new("English 中文 日本語 한국어 العربية Русский 😊");
printf("%s\n", bs_string_c_str(multilingual));
```

```cpp
// C++ version
bs::String multilingual("English 中文 日本語 한국어 العربية Русский 😊");
std::cout << multilingual << std::endl;
```

## Main Features

### String Creation

```c
// C version
bs_string* str1 = bs_string_new("Hello");
bs_string* str2 = bs_string_new_len("Hello World", 5); // Take only first 5 chars
```

```cpp
// C++ version
bs::String str1("Hello");
bs::String str2(std::string("Hello"));
```

### String Conversion

```c
// C version
bs_string* upper = bs_string_to_upper(str);
bs_string* lower = bs_string_to_lower(str);
```

```cpp
// C++ version
bs::String upper = str.toUpperCopy();
str.toLower(); // Modify directly
```

### String Search

```c
// C version
int64_t pos = bs_string_find_cstr(str, "world", 0);
bool contains = bs_string_contains_cstr(str, "hello");
bool starts = bs_string_starts_with(str, prefix);
bool ends = bs_string_ends_with(str, suffix);
```

```cpp
// C++ version
size_t pos = str.find("world");
bool contains = str.contains("hello");
bool starts = str.startsWith(prefix);
bool ends = str.endsWith(suffix);
```

### String Split and Join

```c
// C version
bs_string_array* parts = bs_string_split(str, ",");
bs_string* joined = bs_string_join(parts, "-");
```

```cpp
// C++ version
std::vector<bs::String> parts = str.split(",");
bs::String joined = bs::String::join(parts, "-");
```

## API Reference

For more detailed API documentation, please refer to the following files:

- [API Reference](./api_reference_en.md)

## Performance Considerations

BetterString is designed with high performance in mind, using the following optimization strategies:

1. Lazy evaluation
2. Memory pooling
3. Small string optimization
4. String views
5. Efficient string matching algorithms

## Memory Management

### C Version

The C version of the library requires manual memory management:

```c
bs_string* str = bs_string_new("Hello");
// Use the string...
bs_string_free(str); // Must free
```

### C++ Version

The C++ version uses RAII for automatic memory management:

```cpp
{
    bs::String str("Hello");
    // Use the string...
} // Automatic destruction
``` 