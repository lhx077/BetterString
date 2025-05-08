# BetterString Single-Header Version Usage Guide

## Introduction

To facilitate integration into various projects, BetterString provides a single-header version that does not require compilation into a separate library file. You only need to include the header file to use all functionality. The single-header version has the following advantages:

- No need to link additional library files
- Simplified project build process
- Easy integration into any C/C++ project
- Support for C++-style API even in C projects

## Files

- `single_header/include/better_string_single.h` - C language single-header version
- `single_header/include/better_string_single.hpp` - C++ single-header version

## Using in C Projects

### Basic Usage

```c
// Define the implementation macro in one source file
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.h"

int main() {
    bs_string* str = bs_string_new("Hello, World!");
    printf("%s\n", bs_string_c_str(str));
    bs_string_free(str);
    return 0;
}
```

### Using the C++-Style API

```c
// Define the implementation macro and C++-style API macro in one source file
#define BETTER_STRING_IMPLEMENTATION
#define BETTER_STRING_CPP_STYLE
#include "better_string_single.h"

int main() {
    BsString str = BsNew("Hello, World!");
    printf("%s\n", BsCStr(str));
    
    // Chained operations
    BsString result;
    BsChain_Begin(BsNew("hello"));
    BsChain_Append(" world");
    BsChain_ToUpper();
    BsChain_End();
    printf("%s\n", BsCStr(result));
    
    // New features
    BsString capitalized = BsCapitalize(str);
    BsString reversed = BsReverse(str);
    int words = BsWordCount(str);
    
    BsFree(str);
    BsFree(result);
    BsFree(capitalized);
    BsFree(reversed);
    return 0;
}
```

## Using in C++ Projects

```cpp
// Define the implementation macro in one source file
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.hpp"

int main() {
    bs::String str("Hello, World!");
    std::cout << str << std::endl;
    
    // Chained operations
    str.append(", how are you?").toUpper().trim();
    std::cout << str << std::endl;
    
    // New features
    str.capitalize().reverse();
    std::cout << "Word count: " << str.wordCount() << std::endl;
    
    // STL compatibility
    for (auto c : str) {
        std::cout << static_cast<char>(c);
    }
    std::cout << std::endl;
    
    // Container-like interface
    str.push_back('!');
    str.pop_back();
    
    // STL algorithm compatibility
    std::transform(str.begin(), str.end(), str.begin(), 
                   [](char32_t c) { return std::tolower(c); });
    
    // Automatic memory management, no need to manually free
    return 0;
}
```

## Using in Multi-File Projects

In multi-file projects, define `BETTER_STRING_IMPLEMENTATION` in only one source file, and just include the header in other files:

```c
// File 1: main.c
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.h"

int main() {
    // ...
}
```

```c
// File 2: helper.c
#include "better_string_single.h"

void helper_function() {
    // ...
}
```

## Compilation Considerations

### Compatibility with Other Libraries

If you are using other single-header libraries in your project, it is recommended to place the `BETTER_STRING_IMPLEMENTATION` definition in a separate C file to avoid potential symbol conflicts.

### Thread Safety

The single-header version is not thread-safe by default. If you need to use it in a multi-threaded environment, create separate string objects in each thread or use external synchronization mechanisms.

### Compilation Optimization

When compiling in Release mode, it is recommended to enable optimization for best performance:

- GCC/Clang: `-O2` or `-O3`
- MSVC: `/O2`

## Using CMake

A simplified CMake file `CMakeLists_simple.txt` is provided, which you can use directly or as a reference:

```bash
# Copy or rename the CMake file
cp CMakeLists_simple.txt CMakeLists.txt

# Build the project
mkdir build && cd build
cmake ..
cmake --build .

# Run the examples
./single_header_c_example
./single_header_cpp_example
```

## C++-Style API Reference

The following are the main macros for using the C++-style API in C code:

| C++-Style Macro | Corresponding C Function |
|----------|------------|
| BsNew(str) | bs_string_new(str) |
| BsFree(str) | bs_string_free(str) |
| BsLength(str) | bs_string_length(str) |
| BsCStr(str) | bs_string_c_str(str) |
| BsToUpper(str) | bs_string_to_upper(str) |
| BsToLower(str) | bs_string_to_lower(str) |
| BsContains(str, find) | bs_string_contains(str, find) |
| BsAppend(str, append) | bs_string_append(str, append) |
| BsSplit(str, delimiter) | bs_string_split(str, delimiter) |
| BsFormat(...) | bs_string_format(...) |
| BsCapitalize(str) | bs_string_capitalize(str) |
| BsReverse(str) | bs_string_reverse(str) |
| BsWordCount(str) | bs_string_word_count(str) |

Chained operation macros:

```c
BsChain_Begin(str);
BsChain_Append("text");
BsChain_ToUpper();
BsChain_Trim();
BsChain_Capitalize();
BsChain_Reverse();
BsChain_End();
``` 