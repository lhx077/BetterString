# BetterString - High-Performance Cross-Language String Library

BetterString is a high-performance, feature-rich string processing library available in both C and C++ implementations. It is designed to meet the string processing needs of modern application development, with Unicode support and a comprehensive set of string operations.

## Key Features

- **Cross-Language Support**: Provides both C and C++ versions with unified underlying implementation for consistent performance
- **Unicode Support**: Complete support for UTF-8, UTF-16, and UTF-32 encodings
- **Modern API Design**: C++ version offers fluent chaining calls, C version supports C++-style API
- **Rich Functionality**: Includes searching, replacing, splitting, joining, trimming, and other common string operations
- **STL Compatibility**: Iterators, container interfaces, algorithm support, and hash function support
- **Flexible Integration Options**:
  - Traditional static/dynamic library approach (Regular version)
  - Single-header version, zero-configuration integration into any project (Single-header version)

## Project Structure

To provide maximum flexibility, BetterString is organized into two completely independent sub-projects:

```
BetterString/
├── regular/           # Regular multi-file version
│   ├── c/             # C implementation
│   │   ├── include/   # C headers
│   │   └── src/       # C source code
│   ├── cpp/           # C++ implementation
│   │   ├── include/   # C++ headers
│   │   └── src/       # C++ source code
│   └── examples/      # Regular version examples
│
├── single_header/     # Single-header version
│   ├── include/       # Single headers
│   │   ├── better_string_single.h    # C single header
│   │   └── better_string_single.hpp  # C++ single header
│   └── examples/      # Single-header version examples
│
├── docs/              # Documentation
├── generate_vs2022.bat # VS2022 project generation script
└── CMakeLists.txt     # Super-build CMake file
```

This structure allows you to choose between using the regular version or the single-header version based on your needs, with both versions being completely independent.

## Feature List

BetterString provides a rich set of string manipulation features, including:

### Basic Operations
- String creation, copying, and destruction
- String concatenation and insertion
- Substring extraction
- Finding and replacing
- Case conversion (toUpper/toLower)
- Capitalization (capitalize)
- String reversal (reverse)
- String trimming (trim)

### Advanced Features
- Unicode encoding conversion
- String splitting and joining
- Word counting (wordCount)
- Regular expression matching and replacement
- String formatting
- Numeric conversions

### C++-Specific Features
- Stream operator support
- Method chaining API
- STL-compatible interfaces:
  - Iterators (begin/end)
  - Container interface (at, push_back, pop_back, etc.)
  - Algorithm compatibility
  - std::hash support
- Move semantics support

## Quick Start

### Traditional Library Method (Regular Version)

```cmake
# CMakeLists.txt
# Add subdirectory approach
add_subdirectory(path/to/BetterString/regular)
target_link_libraries(your_target PRIVATE better_string)

# Or use after installation
find_package(BetterString REQUIRED)
target_link_libraries(your_target PRIVATE better_string)
```

### Single-Header Method (C++)

```cpp
// Define implementation macro in one source file
#define BETTER_STRING_IMPLEMENTATION
#include "better_string_single.hpp"

// Usage example
void example() {
    bs::String str("Hello, World!");
    str.append(" Welcome!").toUpper().capitalize();
    std::cout << str << std::endl;
    
    // New feature examples
    int wordCount = str.wordCount();
    str.reverse();
    
    // STL compatibility examples
    for(auto c : str) {
        std::cout << static_cast<char>(c);
    }
    
    std::unordered_map<bs::String, int> stringMap;
    stringMap[str] = 1;
}
```

### Single-Header Method (C - Using C++-Style API)

```c
// Define implementation macro in one source file
#define BETTER_STRING_IMPLEMENTATION
#define BETTER_STRING_CPP_STYLE
#include "better_string_single.h"

// Usage example
void example() {
    BsString str = BsNew("Hello, World!");
    BsChain_Begin(str);
    BsChain_Append(" Welcome!");
    BsChain_ToUpper();
    BsChain_Capitalize();
    BsChain_End();
    
    // New feature examples
    int wordCount = BsWordCount(str);
    str = BsReverse(str);
    
    printf("%s\n", BsCStr(str));
    BsFree(str);
}
```

## Build Methods

### Using CMake

#### Build Both Versions

```bash
# Build from the root directory
mkdir build && cd build
cmake ..
cmake --build .
```

#### Build Regular Version Only

```bash
# Build directly from the regular directory
mkdir build_regular && cd build_regular
cmake ../regular
cmake --build .
```

#### Build Single-Header Version Only

```bash
# Build directly from the single_header directory
mkdir build_single && cd build_single
cmake ../single_header
cmake --build .
```

### Using Visual Studio 2022

```bash
# Run the project's script, which allows you to choose which project type to generate
generate_vs2022.bat
```

The script will guide you to choose between generating the regular version, the single-header version, or both.

## Documentation

For detailed API documentation and usage guides, please refer to the following documents:

- [API Reference](api_reference_en.md)
- [Single-Header Usage Guide](single_header_usage_en.md)
- [Advanced Usage](advanced_usage_en.md)

## Recent Updates

- **STL Compatibility**: Added iterators, container interfaces, algorithm compatibility, and std::hash support for C++ version
- **Project Restructuring**: Completely separated the single-header version and regular multi-file version into two independent sub-projects for a clearer structure
- **New Features**:
  - `capitalize()` - Capitalizes the first letter of each word, making the rest lowercase
  - `reverse()` - Reverses the string content (with support for UTF-8 multi-byte characters)
  - `wordCount()` - Counts the number of words in a string

## Examples

Each version has its own example code:

- Regular version examples are located in [regular/examples](../regular/examples/)
- Single-header version examples are located in [single_header/examples](../single_header/examples/)

## License

This project is licensed under the Apache 2.0 License. 