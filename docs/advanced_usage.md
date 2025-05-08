# BetterString 高级用法指南

本文档介绍 BetterString 库的高级功能和最佳实践，帮助您充分利用库的功能。

## 性能优化

### 内存管理

BetterString 库使用引用计数机制来管理字符串内存。这意味着当您对字符串进行操作时，某些操作会创建新的字符串对象，而某些操作会就地修改现有字符串。

在C语言中，您需要手动管理内存：

```c
bs_string* str = bs_string_new("Hello");
bs_string* upper = bs_string_to_upper(str);
// 使用完毕后释放
bs_string_free(str);
bs_string_free(upper);
```

在C++中，内存管理是自动的，使用RAII原则：

```cpp
bs::String str("Hello");
bs::String upper = str.toUpperCopy();
// 自动释放
```

### 避免不必要的复制

为了提高性能，请避免不必要的字符串复制：

```c
// 不推荐 - 创建了不必要的临时对象
bs_string* temp = bs_string_concat(str1, str2);
bs_string* result = bs_string_concat(temp, str3);
bs_string_free(temp);

// 推荐 - 直接追加
bs_string* result = bs_string_copy(str1);
result = bs_string_append(result, str2);
result = bs_string_append(result, str3);
```

在C++中，链式操作可以帮助减少临时对象：

```cpp
// 高效的链式操作
bs::String result = bs::String(str1).append(str2).append(str3);
```

### 大型字符串操作

对于大型字符串操作（如文件内容处理），使用基于视图的操作可以提高性能：

```c
// TODO: 在库中实现字符串视图功能
```

## Unicode 支持

### 多语言支持

BetterString 完全支持Unicode，可以处理各种语言的字符串：

```c
bs_string* chinese = bs_string_new("你好，世界!");
bs_string* russian = bs_string_new("Привет, мир!");
bs_string* arabic = bs_string_new("مرحبا بالعالم!");
bs_string* emoji = bs_string_new("Hello 😊 World 🌍");
```

### 编码转换

在不同编码之间转换字符串：

```c
// UTF-8 到 UTF-16
bs_string* utf8_str = bs_string_new("Hello, World!");
bs_string* utf16_str = bs_string_to_utf16(utf8_str);

// UTF-16 到 UTF-8
bs_string* back_to_utf8 = bs_string_to_utf8(utf16_str);
```

C++版本：

```cpp
bs::String str("Hello, World!");
std::u16string utf16 = str.toUtf16();
std::u32string utf32 = str.toUtf32();
```

## 高级字符串操作

### 正则表达式

使用正则表达式进行复杂的字符串操作：

```c
// 匹配电子邮件地址
bs_string* text = bs_string_new("Contact us at info@example.com or support@example.com");
bs_string_array* emails = bs_string_regex_find_all(text, "[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");

// 替换电话号码格式
bs_string* phones = bs_string_new("Call 1234567890 or 555-123-4567");
bs_string* formatted = bs_string_replace_regex(phones, "(\\d{3})(\\d{3})(\\d{4})", "($1) $2-$3");
```

C++版本：

```cpp
bs::String text("Contact us at info@example.com or support@example.com");
std::vector<bs::String> emails = text.regexFindAll("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");

bs::String phones("Call 1234567890 or 555-123-4567");
phones.replaceRegex("(\\d{3})(\\d{3})(\\d{4})", "($1) $2-$3");
```

### 格式化字符串

使用格式化函数创建复杂字符串：

```c
bs_string* formatted = bs_string_format("User %s (ID: %d) logged in at %s", "JohnDoe", 12345, "2023-05-15 14:30:00");
```

C++版本：

```cpp
bs::String formatted = bs::String::format("User %s (ID: %d) logged in at %s", "JohnDoe", 12345, "2023-05-15 14:30:00");
```

## 高级迭代

### 使用迭代器

对字符串进行字符级迭代：

```c
bs_string* str = bs_string_new("Hello, 世界!");
bs_iterator* it = bs_string_iterator_new(str);

while (bs_string_iterator_has_next(it)) {
    uint32_t ch = bs_string_iterator_next(it);
    printf("Unicode code point: U+%04X\n", ch);
}

bs_string_iterator_free(it);
bs_string_free(str);
```

## 集成与互操作

### 与标准C字符串互操作

```c
// C字符串到BetterString
const char* c_str = "Hello, World!";
bs_string* bs_str = bs_string_new(c_str);

// BetterString到C字符串
const char* back_to_c = bs_string_c_str(bs_str);
```

### 与C++标准库互操作

```cpp
// std::string到BetterString
std::string std_str = "Hello, World!";
bs::String bs_str(std_str);

// BetterString到std::string
std::string back_to_std = bs_str.toString();

// 与std::vector互操作
std::vector<std::string> std_vec = {"Hello", "World"};
std::vector<bs::String> bs_vec;
for (const auto& s : std_vec) {
    bs_vec.push_back(bs::String(s));
}

// 分割到std::vector
bs::String text("apple,banana,orange");
std::vector<bs::String> fruits = text.split(",");
```

## 单头文件版本的高级用法

### 条件编译

在不同模块中条件编译实现部分：

```c
// 在一个源文件中定义实现
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.h"

// 在其他源文件中只包含接口
#include "better_string_single.h"
```

### 与其他单头文件库一起使用

```c
// 避免命名冲突
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.h"

#define OTHER_LIB_IMPLEMENTATION
#include "other_lib.h"
```

## C++风格的C API

### 使用宏实现链式调用

```c
BsString str = BsNew("hello");

// 使用链式操作
BsChain_Begin(str);
BsChain_Append(", world");
BsChain_ToUpper();
BsChain_Trim();
BsString result = BsChain_End();

printf("%s\n", BsCStr(result)); // 输出: "HELLO, WORLD"
BsFree(result);
BsFree(str);
```

### 命名空间模拟

使用命名风格模拟命名空间：

```c
// 模拟命名空间的风格
BsString str = BsNew("hello");
BsString upper = BsToUpper(str);
bool contains = BsContains(str, "el");
```

## 与现代C/C++功能结合

### 与C11/C17特性结合

```c
// 使用C11通用选择表达式
#include <stdbool.h>

#define SELECT_BY_ENCODING(str, utf8, utf16, utf32) _Generic((bs_string_encoding(str)), \
    BS_ENCODING_UTF8: (utf8), \
    BS_ENCODING_UTF16: (utf16), \
    BS_ENCODING_UTF32: (utf32), \
    default: (utf8))

// 使用示例
const char* encoding_name = SELECT_BY_ENCODING(str, "UTF-8", "UTF-16", "UTF-32");
```

### 与C++17/C++20特性结合

```cpp
// 使用C++17结构化绑定
bs::String text("key=value");
auto parts = text.split("=");
if (parts.size() == 2) {
    auto [key, value] = std::tie(parts[0], parts[1]);
    std::cout << "Key: " << key << ", Value: " << value << std::endl;
}

// 使用C++20范围
#include <ranges>
bs::String str("Hello, World!");
auto words = str.split(" ");
for (const auto& word : words) {
    std::cout << word << std::endl;
}
```

## 最佳实践

### 内存管理

- 在C中始终记得释放字符串对象
- 使用适当的作用域管理C++对象生命周期
- 避免不必要的复制和临时对象

### 性能考虑

- 对于频繁修改的字符串，考虑一次性分配足够的空间
- 对于只读操作，使用字符串视图而不是复制
- 对于大型字符串处理，考虑分块处理避免一次性加载全部内容

### 错误处理

- 检查返回的指针是否为NULL
- 在C++中使用异常处理捕获错误
- 优雅地处理编码错误和转换失败

## 总结

BetterString库提供了丰富的功能集，可以满足现代应用程序的字符串处理需求。通过利用本指南中的高级用法和最佳实践，您可以充分发挥库的潜力，编写更高效、更可靠的代码。 