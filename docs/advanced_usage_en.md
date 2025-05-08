# BetterString Advanced Usage Guide

This document covers advanced usage patterns and techniques for both C and C++ implementations of BetterString.

## Memory Management

### C API Memory Management

When using the C API, memory management is manual:

```c
// Create a string
bs_string* str = bs_string_new("Hello, World!");

// Use the string...

// Don't forget to free the string when done
bs_string_free(str);
```

Be careful with functions that return new strings. You must free each string individually:

```c
bs_string* str = bs_string_new("Hello");
bs_string* upper = bs_string_to_upper(str);

// Use both strings...

// Free both strings
bs_string_free(upper);
bs_string_free(str);
```

### C++ API Memory Management

The C++ API handles memory management automatically through RAII (Resource Acquisition Is Initialization):

```cpp
// Scope 1
{
    bs::String str("Hello, World!");
    // Use the string...
} // str is automatically freed when it goes out of scope

// Scope 2
{
    bs::String str1("Hello");
    bs::String str2 = str1.toUpperCopy();
    // Both str1 and str2 are automatically freed
}
```

## Unicode Handling

BetterString supports UTF-8, UTF-16, and UTF-32 encodings:

### Unicode in C API

```c
// Creating strings with different encodings
uint16_t utf16_str[] = {0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0}; // "Hello" in UTF-16
uint32_t utf32_str[] = {0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0}; // "Hello" in UTF-32

bs_string* str1 = bs_string_new_utf16(utf16_str);
bs_string* str2 = bs_string_new_utf32(utf32_str);

// Get encoding of a string
bs_encoding_t encoding = bs_string_encoding(str1);

// Character count vs byte length
size_t char_count = bs_string_length(str1);
size_t byte_count = bs_string_byte_length(str1);

// Accessing characters
uint32_t ch = bs_string_char_at(str1, 1); // Get the second character

// Iterating over characters
bs_iterator* it = bs_string_iterator_new(str1);
while (bs_string_iterator_has_next(it)) {
    uint32_t ch = bs_string_iterator_next(it);
    // Process each character...
}
bs_string_iterator_free(it);

// Cleanup
bs_string_free(str1);
bs_string_free(str2);
```

### Unicode in C++ API

```cpp
// Creating strings
bs::String str1 = bs::String::fromUtf16(u"Hello"); // from UTF-16
bs::String str2 = bs::String::fromUtf32(U"Hello"); // from UTF-32

// Converting to different encodings
std::u16string utf16 = str1.toUtf16();
std::u32string utf32 = str1.toUtf32();

// Iterating over characters
for (char32_t ch : str1) {
    // Process each Unicode code point
}

// Access individual characters
char32_t ch = str1.charAt(1); // Get the second character
```

## Regular Expression Support

BetterString provides basic regular expression functionality:

### Regex in C API

```c
bs_string* str = bs_string_new("Hello, World! Welcome to BetterString.");

// Check if a string matches a pattern
bool matches = bs_string_matches(str, "Hello.*");

// Replace using regex
bs_string* replaced = bs_string_replace_regex(str, "World", "Universe");

// Split using regex
bs_string_array* parts = bs_string_regex_split(str, "[,!]");

// Find all matches
bs_string_array* matches = bs_string_regex_find_all(str, "\\w+");

// Process matches
for (size_t i = 0; i < matches->length; i++) {
    bs_string* match = matches->items[i];
    printf("%s\n", bs_string_c_str(match));
}

// Cleanup
bs_string_array_free(parts);
bs_string_array_free(matches);
bs_string_free(replaced);
bs_string_free(str);
```

### Regex in C++ API

```cpp
bs::String str("Hello, World! Welcome to BetterString.");

// Check if a string matches a pattern
bool matches = str.matches("Hello.*");

// Replace using regex
bs::String replaced = str.replaceRegex("World", "Universe");

// Split using regex
std::vector<bs::String> parts = str.regexSplit("[,!]");

// Find all matches
std::vector<bs::String> matches = str.regexFindAll("\\w+");

// Process matches
for (const auto& match : matches) {
    std::cout << match << std::endl;
}
```

## String Building and Formatting

### String Building in C API

```c
// Using bs_string_append_cstr for efficient building
bs_string* builder = bs_string_new("");
bs_string_append_cstr(builder, "Hello, ");
bs_string_append_cstr(builder, "World");
bs_string_append_cstr(builder, "!");

// Using format
bs_string* formatted = bs_string_format("Hello, %s! Count: %d", "User", 42);

// Using C++-style API for chaining in C
#ifdef BETTER_STRING_CPP_STYLE
BsString str;
BsChain_Begin(BsNew(""));
BsChain_Append("Hello, ");
BsChain_Append("World");
BsChain_Append("!");
BsChain_ToUpper();
BsChain_End();
#endif

// Cleanup
bs_string_free(builder);
bs_string_free(formatted);
#ifdef BETTER_STRING_CPP_STYLE
BsFree(str);
#endif
```

### String Building in C++ API

```cpp
// Using method chaining
bs::String str = bs::String("Hello")
    .append(", ")
    .append("World")
    .append("!")
    .toUpper();

// Using format
bs::String formatted = bs::String::format("Hello, %s! Count: %d", "User", 42);
```

## String Views

String views allow you to work with portions of strings without creating new string objects:

### String Views in C API

```c
bs_string* original = bs_string_new("Hello, World!");

// Create a view (does not copy data)
// Note: In the current implementation, the view functionality is limited
// and mainly used internally. Use substring for safer operations.
bs_string* view = bs_string_substring(original, 7, 5); // "World"

// Use the view
printf("%s\n", bs_string_c_str(view));

// Free both strings
bs_string_free(view);
bs_string_free(original);
```

### Working with Substrings in C++ API

```cpp
bs::String original("Hello, World!");

// Create a substring (this creates a new string)
bs::String sub = original.substring(7, 5); // "World"

// Use the substring
std::cout << sub << std::endl;
```

## Working with String Arrays

### String Arrays in C API

```c
bs_string* str = bs_string_new("apple,banana,cherry,date");

// Split into array
bs_string_array* fruits = bs_string_split(str, ",");

// Access items in the array
for (size_t i = 0; i < fruits->length; i++) {
    bs_string* fruit = fruits->items[i];
    printf("%s\n", bs_string_c_str(fruit));
}

// Join array into a new string
bs_string* joined = bs_string_join(fruits, " - ");

// Don't forget to free arrays when done
bs_string_array_free(fruits);
bs_string_free(joined);
bs_string_free(str);
```

### String Arrays in C++ API

```cpp
bs::String str("apple,banana,cherry,date");

// Split into vector
std::vector<bs::String> fruits = str.split(",");

// Access items in the vector
for (const auto& fruit : fruits) {
    std::cout << fruit << std::endl;
}

// Join vector into a new string
bs::String joined = bs::String::join(fruits, " - ");
```

## STL Integration (C++ Only)

BetterString's C++ implementation offers seamless integration with STL containers and algorithms:

### Using STL Algorithms

```cpp
#include <algorithm>
#include <vector>
#include "better_string.hpp"

void stlAlgorithmExamples() {
    bs::String str("Hello, World!");
    
    // Check if all characters are ASCII
    bool allAscii = std::all_of(str.begin(), str.end(), 
                              [](char32_t c) { return c < 128; });
    
    // Count uppercase letters
    int upperCount = std::count_if(str.begin(), str.end(), 
                                 [](char32_t c) { return c >= 'A' && c <= 'Z'; });
    
    // Transform to lowercase using STL algorithm
    std::string result;
    result.reserve(str.length());
    std::transform(str.begin(), str.end(), std::back_inserter(result),
                 [](char32_t c) { return static_cast<char>(std::tolower(c)); });
}
```

### Using with STL Containers

```cpp
#include <map>
#include <unordered_map>
#include <vector>
#include "better_string.hpp"

void stlContainerExamples() {
    // Vector of strings
    std::vector<bs::String> names;
    names.push_back("Alice");
    names.push_back("Bob");
    names.push_back("Charlie");
    
    // Map with string keys
    std::map<bs::String, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;
    
    // Unordered map (requires std::hash specialization, which is provided)
    std::unordered_map<bs::String, int> quickLookup;
    quickLookup["Alice"] = 95;
    quickLookup["Bob"] = 87;
    
    // Access by iterator
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    
    // Range-based for loop
    for (const auto& pair : scores) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}
```

## Performance Considerations

### Memory Allocation

- **String Concatenation**: When concatenating many strings, it's more efficient to use a building pattern rather than repeated concatenation.
- **C API**: Use `bs_string_append_cstr` on a single string rather than multiple `bs_string_concat` calls.
- **C++ API**: Use chained `append` calls rather than the `+` operator for multiple concatenations.

### Working with Large Strings

When working with large strings, consider the following:

- Use iterators to process strings character by character without creating temporary strings.
- For C API, use `bs_string_substring` or `bs_string_iterator` for processing substrings.
- For C++ API, use the iterator interface with STL algorithms for efficient processing.

## Thread Safety

The BetterString library is not inherently thread-safe. If you need to use BetterString objects across multiple threads:

- **C API**: Do not share `bs_string` objects between threads without external synchronization.
- **C++ API**: Do not share `bs::String` objects between threads without proper synchronization mechanisms (mutex, lock, etc.).

## Integration with Other Libraries

### C Integration

```c
// Example: Integrating with a C library that uses const char*
extern void some_c_function(const char* str);

bs_string* my_string = bs_string_new("Hello, Integration!");
some_c_function(bs_string_c_str(my_string));
bs_string_free(my_string);
```

### C++ Integration

```cpp
// Example: Integrating with a C++ library that uses std::string
extern void some_cpp_function(const std::string& str);

bs::String my_string("Hello, Integration!");
some_cpp_function(my_string.toString());
// No explicit cleanup needed
```

## Using Single-Header vs. Regular Version

### When to Use the Single-Header Version

- For small projects or quick prototypes
- When you want to minimize build complexity
- When distributing code that needs to work without dependencies

### When to Use the Regular Version

- For larger projects with multiple files
- When you need the best compile-time performance
- When integrating with a complex build system

## Custom Builds

You can customize the BetterString library by modifying CMake options:

```bash
# Regular version with default options
cmake -S ./regular -B ./build_regular

# Single-header version with examples disabled
cmake -S ./single_header -B ./build_single -DBETTER_STRING_BUILD_EXAMPLES=OFF

# Both versions with testing enabled
cmake -S . -B ./build -DBETTER_STRING_BUILD_TESTS=ON
```

## Debugging Tips

If you encounter issues with BetterString:

1. **Memory Leaks**: Ensure all `bs_string` objects are properly freed in C code.
2. **String Views**: Be careful with string views, as they reference another string's memory.
3. **Unicode Issues**: Verify that your strings use the correct encoding. 