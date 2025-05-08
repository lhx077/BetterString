# BetterString 高级使用指南

本文档涵盖 BetterString 库的 C 和 C++ 实现的高级使用模式和技术。

## 内存管理

### C API 内存管理

使用 C API 时，内存管理是手动的：

```c
// 创建字符串
bs_string* str = bs_string_new("Hello, World!");

// 使用字符串...

// 不要忘记在使用完后释放字符串
bs_string_free(str);
```

注意返回新字符串的函数。您必须单独释放每个字符串：

```c
bs_string* str = bs_string_new("Hello");
bs_string* upper = bs_string_to_upper(str);

// 使用两个字符串...

// 释放两个字符串
bs_string_free(upper);
bs_string_free(str);
```

### C++ API 内存管理

C++ API 通过 RAII（资源获取即初始化）自动处理内存管理：

```cpp
// 作用域 1
{
    bs::String str("Hello, World!");
    // 使用字符串...
} // 当 str 超出作用域时自动释放

// 作用域 2
{
    bs::String str1("Hello");
    bs::String str2 = str1.toUpperCopy();
    // str1 和 str2 都会自动释放
}
```

## Unicode 处理

BetterString 支持 UTF-8、UTF-16 和 UTF-32 编码：

### C API 中的 Unicode

```c
// 使用不同编码创建字符串
uint16_t utf16_str[] = {0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0}; // UTF-16 中的 "Hello"
uint32_t utf32_str[] = {0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0}; // UTF-32 中的 "Hello"

bs_string* str1 = bs_string_new_utf16(utf16_str);
bs_string* str2 = bs_string_new_utf32(utf32_str);

// 获取字符串的编码
bs_encoding_t encoding = bs_string_encoding(str1);

// 字符数与字节长度
size_t char_count = bs_string_length(str1);
size_t byte_count = bs_string_byte_length(str1);

// 访问字符
uint32_t ch = bs_string_char_at(str1, 1); // 获取第二个字符

// 迭代字符
bs_iterator* it = bs_string_iterator_new(str1);
while (bs_string_iterator_has_next(it)) {
    uint32_t ch = bs_string_iterator_next(it);
    // 处理每个字符...
}
bs_string_iterator_free(it);

// 清理
bs_string_free(str1);
bs_string_free(str2);
```

### C++ API 中的 Unicode

```cpp
// 创建字符串
bs::String str1 = bs::String::fromUtf16(u"Hello"); // 从 UTF-16
bs::String str2 = bs::String::fromUtf32(U"Hello"); // 从 UTF-32

// 转换为不同编码
std::u16string utf16 = str1.toUtf16();
std::u32string utf32 = str1.toUtf32();

// 迭代字符
for (char32_t ch : str1) {
    // 处理每个 Unicode 码点
}

// 访问单个字符
char32_t ch = str1.charAt(1); // 获取第二个字符
```

## 正则表达式支持

BetterString 提供基本的正则表达式功能：

### C API 中的正则表达式

```c
bs_string* str = bs_string_new("Hello, World! Welcome to BetterString.");

// 检查字符串是否匹配模式
bool matches = bs_string_matches(str, "Hello.*");

// 使用正则表达式替换
bs_string* replaced = bs_string_replace_regex(str, "World", "Universe");

// 使用正则表达式分割
bs_string_array* parts = bs_string_regex_split(str, "[,!]");

// 查找所有匹配项
bs_string_array* matches = bs_string_regex_find_all(str, "\\w+");

// 处理匹配项
for (size_t i = 0; i < matches->length; i++) {
    bs_string* match = matches->items[i];
    printf("%s\n", bs_string_c_str(match));
}

// 清理
bs_string_array_free(parts);
bs_string_array_free(matches);
bs_string_free(replaced);
bs_string_free(str);
```

### C++ API 中的正则表达式

```cpp
bs::String str("Hello, World! Welcome to BetterString.");

// 检查字符串是否匹配模式
bool matches = str.matches("Hello.*");

// 使用正则表达式替换
bs::String replaced = str.replaceRegex("World", "Universe");

// 使用正则表达式分割
std::vector<bs::String> parts = str.regexSplit("[,!]");

// 查找所有匹配项
std::vector<bs::String> matches = str.regexFindAll("\\w+");

// 处理匹配项
for (const auto& match : matches) {
    std::cout << match << std::endl;
}
```

## 字符串构建和格式化

### C API 中的字符串构建

```c
// 使用 bs_string_append_cstr 进行高效构建
bs_string* builder = bs_string_new("");
bs_string_append_cstr(builder, "Hello, ");
bs_string_append_cstr(builder, "World");
bs_string_append_cstr(builder, "!");

// 使用格式化
bs_string* formatted = bs_string_format("Hello, %s! Count: %d", "User", 42);

// 在 C 中使用 C++ 风格 API 进行链式操作
#ifdef BETTER_STRING_CPP_STYLE
BsString str;
BsChain_Begin(BsNew(""));
BsChain_Append("Hello, ");
BsChain_Append("World");
BsChain_Append("!");
BsChain_ToUpper();
BsChain_End();
#endif

// 清理
bs_string_free(builder);
bs_string_free(formatted);
#ifdef BETTER_STRING_CPP_STYLE
BsFree(str);
#endif
```

### C++ API 中的字符串构建

```cpp
// 使用方法链
bs::String str = bs::String("Hello")
    .append(", ")
    .append("World")
    .append("!")
    .toUpper();

// 使用格式化
bs::String formatted = bs::String::format("Hello, %s! Count: %d", "User", 42);
```

## 字符串视图

字符串视图允许您操作字符串的部分内容而无需创建新的字符串对象：

### C API 中的字符串视图

```c
bs_string* original = bs_string_new("Hello, World!");

// 创建视图（不复制数据）
// 注意：在当前实现中，视图功能有限
// 主要用于内部使用。更安全的操作请使用子字符串。
bs_string* view = bs_string_substring(original, 7, 5); // "World"

// 使用视图
printf("%s\n", bs_string_c_str(view));

// 释放两个字符串
bs_string_free(view);
bs_string_free(original);
```

### C++ API 中处理子字符串

```cpp
bs::String original("Hello, World!");

// 创建子字符串（这会创建一个新字符串）
bs::String sub = original.substring(7, 5); // "World"

// 使用子字符串
std::cout << sub << std::endl;
```

## 处理字符串数组

### C API 中的字符串数组

```c
bs_string* str = bs_string_new("apple,banana,cherry,date");

// 分割成数组
bs_string_array* fruits = bs_string_split(str, ",");

// 访问数组中的项
for (size_t i = 0; i < fruits->length; i++) {
    bs_string* fruit = fruits->items[i];
    printf("%s\n", bs_string_c_str(fruit));
}

// 将数组连接成新字符串
bs_string* joined = bs_string_join(fruits, " - ");

// 不要忘记在完成后释放数组
bs_string_array_free(fruits);
bs_string_free(joined);
bs_string_free(str);
```

### C++ API 中的字符串数组

```cpp
bs::String str("apple,banana,cherry,date");

// 分割成向量
std::vector<bs::String> fruits = str.split(",");

// 访问向量中的项
for (const auto& fruit : fruits) {
    std::cout << fruit << std::endl;
}

// 将向量连接成新字符串
bs::String joined = bs::String::join(fruits, " - ");
```

## STL 集成（仅 C++）

BetterString 的 C++ 实现提供与 STL 容器和算法的无缝集成：

### 使用 STL 算法

```cpp
#include <algorithm>
#include <vector>
#include "better_string.hpp"

void stlAlgorithmExamples() {
    bs::String str("Hello, World!");
    
    // 检查所有字符是否为 ASCII
    bool allAscii = std::all_of(str.begin(), str.end(), 
                              [](char32_t c) { return c < 128; });
    
    // 计算大写字母数量
    int upperCount = std::count_if(str.begin(), str.end(), 
                                 [](char32_t c) { return c >= 'A' && c <= 'Z'; });
    
    // 使用 STL 算法转换为小写
    std::string result;
    result.reserve(str.length());
    std::transform(str.begin(), str.end(), std::back_inserter(result),
                 [](char32_t c) { return static_cast<char>(std::tolower(c)); });
}
```

### 与 STL 容器一起使用

```cpp
#include <map>
#include <unordered_map>
#include <vector>
#include "better_string.hpp"

void stlContainerExamples() {
    // 字符串向量
    std::vector<bs::String> names;
    names.push_back("Alice");
    names.push_back("Bob");
    names.push_back("Charlie");
    
    // 使用字符串键的映射
    std::map<bs::String, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;
    
    // 无序映射（需要 std::hash 特化，已提供）
    std::unordered_map<bs::String, int> quickLookup;
    quickLookup["Alice"] = 95;
    quickLookup["Bob"] = 87;
    
    // 通过迭代器访问
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    
    // 基于范围的 for 循环
    for (const auto& pair : scores) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}
```

## 性能考虑

### 内存分配

- **字符串连接**：连接多个字符串时，使用构建模式比重复连接更高效。
- **C API**：在单个字符串上使用 `bs_string_append_cstr` 而不是多次调用 `bs_string_concat`。
- **C++ API**：对于多次连接，使用链式 `append` 调用而不是 `+` 运算符。

### 处理大型字符串

处理大型字符串时，请考虑以下事项：

- 使用迭代器逐字符处理字符串，无需创建临时字符串。
- 对于 C API，使用 `bs_string_substring` 或 `bs_string_iterator` 处理子字符串。
- 对于 C++ API，使用带有 STL 算法的迭代器接口进行高效处理。

## 线程安全

BetterString 库本身不是线程安全的。如果需要在多个线程之间使用 BetterString 对象：

- **C API**：不要在没有外部同步的情况下在线程之间共享 `bs_string` 对象。
- **C++ API**：不要在没有适当同步机制（互斥锁、锁等）的情况下在线程之间共享 `bs::String` 对象。

## 与其他库集成

### C 集成

```c
// 示例：与使用 const char* 的 C 库集成
extern void some_c_function(const char* str);

bs_string* my_string = bs_string_new("Hello, Integration!");
some_c_function(bs_string_c_str(my_string));
bs_string_free(my_string);
```

### C++ 集成

```cpp
// 示例：与使用 std::string 的 C++ 库集成
extern void some_cpp_function(const std::string& str);

bs::String my_string("Hello, Integration!");
some_cpp_function(my_string.toString());
// 不需要显式清理
```

## 使用单头文件版本与常规版本

### 何时使用单头文件版本

- 对于小型项目或快速原型
- 当您想要最小化构建复杂性时
- 当分发需要在没有依赖关系的情况下工作的代码时

### 何时使用常规版本

- 对于具有多个文件的大型项目
- 当您需要最佳的编译时性能时
- 当与复杂的构建系统集成时

## 自定义构建

您可以通过修改 CMake 选项来自定义 BetterString 库：

```bash
# 默认选项的常规版本
cmake -S ./regular -B ./build_regular

# 禁用示例的单头文件版本
cmake -S ./single_header -B ./build_single -DBETTER_STRING_BUILD_EXAMPLES=OFF

# 启用测试的两个版本
cmake -S . -B ./build -DBETTER_STRING_BUILD_TESTS=ON
```

## 调试技巧

如果您遇到 BetterString 的问题：

1. **内存泄漏**：确保在 C 代码中正确释放所有 `bs_string` 对象。
2. **字符串视图**：小心使用字符串视图，因为它们引用另一个字符串的内存。
3. **Unicode 问题**：验证您的字符串使用正确的编码。 