# BetterString API Reference

This document provides a comprehensive reference for the BetterString library API, covering both C and C++ interfaces.

## C API

### Basic Types

```c
typedef enum {
    BS_ENCODING_UTF8,    // UTF-8 encoding
    BS_ENCODING_UTF16,   // UTF-16 encoding
    BS_ENCODING_UTF32,   // UTF-32 encoding
    BS_ENCODING_ASCII    // ASCII encoding
} bs_encoding_t;

// String object type
typedef struct bs_string {
    uint8_t* data;           // Raw byte data
    size_t length;           // Character count (not byte length)
    size_t byte_length;      // Byte length
    bs_encoding_t encoding;  // Encoding type
    bool is_view;            // Whether it's a view (doesn't own memory)
} bs_string;

// String array type
typedef struct bs_string_array {
    bs_string** items;       // String array
    size_t length;           // Array length
} bs_string_array;

// Iterator type
typedef struct bs_iterator {
    const bs_string* str;    // The string being iterated
    size_t position;         // Current byte position
    size_t char_position;    // Current character position
} bs_iterator;
```

### Constructor and Destructor Functions

```c
// Create a new string from a C string
bs_string* bs_string_new(const char* str);

// Create a new string from a C string with specified length
bs_string* bs_string_new_len(const char* str, size_t length);

// Create a new string from a UTF-16 string
bs_string* bs_string_new_utf16(const uint16_t* str);

// Create a new string from a UTF-32 string
bs_string* bs_string_new_utf32(const uint32_t* str);

// Create a new string with specified encoding
bs_string* bs_string_new_with_encoding(const void* str, size_t byte_length, bs_encoding_t encoding);

// Create a copy of an existing string
bs_string* bs_string_copy(const bs_string* str);

// Free a string
void bs_string_free(bs_string* str);
```

### Property Access Functions

```c
// Get the length of a string in characters
size_t bs_string_length(const bs_string* str);

// Get the length of a string in bytes
size_t bs_string_byte_length(const bs_string* str);

// Get the encoding of a string
bs_encoding_t bs_string_encoding(const bs_string* str);

// Get a null-terminated C string
const char* bs_string_c_str(const bs_string* str);
```

### String Operation Functions

```c
// Extract a substring
bs_string* bs_string_substring(const bs_string* str, size_t start, size_t length);

// Concatenate two strings
bs_string* bs_string_concat(const bs_string* str1, const bs_string* str2);

// Append a string to another
bs_string* bs_string_append(bs_string* str, const bs_string* append);

// Append a C string to a string
bs_string* bs_string_append_cstr(bs_string* str, const char* append);

// Insert a string at a specified position
bs_string* bs_string_insert(bs_string* str, size_t position, const bs_string* insert);

// Delete a portion of a string
bs_string* bs_string_delete(bs_string* str, size_t start, size_t length);

// Replace a substring with another string
bs_string* bs_string_replace(bs_string* str, const bs_string* find, const bs_string* replace);
```

### Modification Functions

```c
// Trim whitespace from both ends of a string
bs_string* bs_string_trim(const bs_string* str);

// Trim whitespace from the start of a string
bs_string* bs_string_trim_start(const bs_string* str);

// Trim whitespace from the end of a string
bs_string* bs_string_trim_end(const bs_string* str);

// Convert a string to uppercase
bs_string* bs_string_to_upper(const bs_string* str);

// Convert a string to lowercase
bs_string* bs_string_to_lower(const bs_string* str);

// Capitalize each word in a string (first letter uppercase, rest lowercase)
bs_string* bs_string_capitalize(const bs_string* str);

// Reverse the string content
bs_string* bs_string_reverse(const bs_string* str);
```

### Search and Comparison Functions

```c
// Find a substring within a string
int64_t bs_string_find(const bs_string* str, const bs_string* find, size_t start_pos);

// Find a C string within a string
int64_t bs_string_find_cstr(const bs_string* str, const char* find, size_t start_pos);

// Find a substring within a string, searching from the end
int64_t bs_string_rfind(const bs_string* str, const bs_string* find, size_t start_pos);

// Check if a string contains a substring
bool bs_string_contains(const bs_string* str, const bs_string* find);

// Check if a string contains a C string
bool bs_string_contains_cstr(const bs_string* str, const char* find);

// Check if a string starts with a prefix
bool bs_string_starts_with(const bs_string* str, const bs_string* prefix);

// Check if a string ends with a suffix
bool bs_string_ends_with(const bs_string* str, const bs_string* suffix);

// Compare two strings
int bs_string_compare(const bs_string* str1, const bs_string* str2);

// Check if two strings are equal
bool bs_string_equals(const bs_string* str1, const bs_string* str2);

// Check if two strings are equal, ignoring case
bool bs_string_equals_ignore_case(const bs_string* str1, const bs_string* str2);
```

### Splitting and Joining Functions

```c
// Split a string by a delimiter
bs_string_array* bs_string_split(const bs_string* str, const char* delimiter);

// Join an array of strings with a delimiter
bs_string* bs_string_join(const bs_string_array* array, const char* delimiter);

// Free a string array
void bs_string_array_free(bs_string_array* array);
```

### Utility Functions

```c
// Count the number of words in a string
size_t bs_string_word_count(const bs_string* str);

// Get a character at a specified index
uint32_t bs_string_char_at(const bs_string* str, size_t index);

// Get a character at a specified index as a string
bs_string* bs_string_char_at_str(const bs_string* str, size_t index);

// Create a new iterator for a string
bs_iterator* bs_string_iterator_new(const bs_string* str);

// Check if an iterator has more characters
bool bs_string_iterator_has_next(const bs_iterator* it);

// Get the next character from an iterator
uint32_t bs_string_iterator_next(bs_iterator* it);

// Free an iterator
void bs_string_iterator_free(bs_iterator* it);
```

### Conversion Functions

```c
// Create a string from an integer
bs_string* bs_string_from_int(int64_t value);

// Create a string from a floating-point number
bs_string* bs_string_from_float(double value);

// Convert a string to an integer
int64_t bs_string_to_int(const bs_string* str);

// Convert a string to a floating-point number
double bs_string_to_float(const bs_string* str);

// Try to parse a string as an integer
bool bs_string_try_parse_int(const bs_string* str, int64_t* value);

// Try to parse a string as a floating-point number
bool bs_string_try_parse_float(const bs_string* str, double* value);
```

### Regular Expression Functions

```c
// Check if a string matches a pattern
bool bs_string_matches(const bs_string* str, const char* pattern);

// Replace substrings matching a pattern with a replacement
bs_string* bs_string_replace_regex(const bs_string* str, const char* pattern, const char* replacement);

// Split a string by a regex pattern
bs_string_array* bs_string_regex_split(const bs_string* str, const char* pattern);

// Find all substrings matching a pattern
bs_string_array* bs_string_regex_find_all(const bs_string* str, const char* pattern);
```

### Formatting Functions

```c
// Format a string using printf-style format specifiers
bs_string* bs_string_format(const char* format, ...);
```

## C++-Style C API

When the `BETTER_STRING_CPP_STYLE` macro is defined, a set of C++-style macros is available for C programs:

```c
// String type definition
typedef bs_string* BsString;

// Construction macros
#define BsNew(str) bs_string_new(str)
#define BsNewLen(str, len) bs_string_new_len(str, len)
#define BsNewUtf16(str) bs_string_new_utf16(str)
#define BsNewUtf32(str) bs_string_new_utf32(str)
#define BsCopy(str) bs_string_copy(str)

// Destruction macro
#define BsFree(str) bs_string_free(str)

// Property access macros
#define BsLength(str) bs_string_length(str)
#define BsByteLength(str) bs_string_byte_length(str)
#define BsEncoding(str) bs_string_encoding(str)
#define BsCStr(str) bs_string_c_str(str)
#define BsIsEmpty(str) (bs_string_length(str) == 0)

// Operation macros
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

// Chain operation macros
#define BsChain_Begin(str) do { BsString _bs_temp = (str);
#define BsChain_End() _bs_temp; } while(0)
#define BsChain_Append(append) _bs_temp = BsAppendCStr(_bs_temp, append)
#define BsChain_ToUpper() _bs_temp = BsToUpper(_bs_temp)
#define BsChain_ToLower() _bs_temp = BsToLower(_bs_temp)
#define BsChain_Trim() _bs_temp = BsTrim(_bs_temp)
#define BsChain_Capitalize() _bs_temp = BsCapitalize(_bs_temp)
#define BsChain_Reverse() _bs_temp = BsReverse(_bs_temp)
```

## C++ API

### Class Definition

```cpp
namespace bs {

class String {
public:
    // Constructors & Destructor
    String();
    String(const char* str);
    String(const std::string& str);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    // Assignment operators
    String& operator=(const char* str);
    String& operator=(const std::string& str);
    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;

    // STL Compatibility - Iterator Support
    class CharIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = char32_t;
        using pointer = char32_t*;
        using reference = char32_t;
        
        CharIterator(const bs_string* str, size_t pos = 0);
        CharIterator& operator++();
        CharIterator operator++(int);
        CharIterator& operator--();
        CharIterator operator--(int);
        reference operator*() const;
        bool operator==(const CharIterator& other) const;
        bool operator!=(const CharIterator& other) const;
    };
    
    // Iterator methods
    CharIterator begin() const;
    CharIterator end() const;
    
    // STL Compatibility - Container Interface
    char32_t at(size_t pos) const;
    void push_back(char32_t ch);
    void pop_back();
    void clear();
    size_t size() const;
    bool empty() const;

    // Basic operations
    String& append(const char* str);
    String& append(const String& other);
    String& append(const std::string& str);
    String& insert(size_t pos, const char* str);
    String& insert(size_t pos, const String& other);
    String& replace(const String& from, const String& to);
    String& replaceAll(const String& from, const String& to);
    String& erase(size_t pos, size_t len = std::string::npos);

    // Property access
    size_t length() const;
    size_t byteLength() const;
    bool isEmpty() const;
    size_t wordCount() const;
    const char* c_str() const;
    std::string toString() const;
    std::u16string toUtf16() const;
    std::u32string toUtf32() const;

    // Substring extraction
    String substring(size_t start, size_t length = std::string::npos) const;

    // Modification methods
    String& trim();
    String& trimLeft();
    String& trimRight();
    String& toUpper();
    String& toLower();
    String& capitalize();
    String& reverse();

    // Non-modifying copy methods
    String trimCopy() const;
    String toUpperCopy() const;
    String toLowerCopy() const;
    String capitalizeCopy() const;
    String reverseCopy() const;

    // Search methods
    size_t find(const String& str, size_t startPos = 0) const;
    size_t find(const char* str, size_t startPos = 0) const;
    size_t rfind(const String& str, size_t startPos = std::string::npos) const;
    bool contains(const String& str) const;
    bool contains(const char* str) const;
    bool startsWith(const String& prefix) const;
    bool endsWith(const String& suffix) const;

    // Comparison
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;
    bool operator<(const String& other) const;
    bool operator<=(const String& other) const;
    bool operator>(const String& other) const;
    bool operator>=(const String& other) const;
    int compare(const String& other) const;
    bool equals(const String& other, bool ignoreCase = false) const;

    // Operators
    String operator+(const String& other) const;
    String operator+(const char* str) const;
    String operator+(const std::string& str) const;
    char operator[](size_t pos) const;
    char& operator[](size_t pos);

    // Splitting and joining
    std::vector<String> split(const String& delimiter) const;
    std::vector<String> split(const char* delimiter) const;
    static String join(const std::vector<String>& strings, const String& delimiter);

    // Character access
    char32_t charAt(size_t index) const;
    String charAtAsString(size_t index) const;

    // Conversion methods
    static String fromInt(int64_t value);
    static String fromFloat(double value);
    int64_t toInt() const;
    double toFloat() const;
    bool tryParseInt(int64_t& value) const;
    bool tryParseFloat(double& value) const;

    // Regular expression methods
    bool matches(const String& pattern) const;
    String& replaceRegex(const String& pattern, const String& replacement);
    std::vector<String> regexSplit(const String& pattern) const;
    std::vector<String> regexFindAll(const String& pattern) const;

    // Formatting
    static String format(const char* format, ...);

private:
    bs_string* m_str; // Internal C string type
};

// Non-member functions
String operator+(const char* lhs, const String& rhs);
String operator+(const std::string& lhs, const String& rhs);
std::ostream& operator<<(std::ostream& os, const String& str);
std::istream& operator>>(std::istream& is, String& str);

} // namespace bs

// STL compatibility - std::hash specialization
namespace std {
    template <>
    struct hash<bs::String> {
        size_t operator()(const bs::String& str) const;
    };
}
```

## Version Differences

### Regular Version vs. Single-Header Version

Both versions provide identical APIs with the following differences:

1. **Header Files:**
   - Regular version: Include `better_string.h` (C) or `better_string.hpp` (C++)
   - Single-header version: Include `better_string_single.h` (C) or `better_string_single.hpp` (C++)

2. **Usage:**
   - Regular version: Link against the library
   - Single-header version: Define `BETTER_STRING_IMPLEMENTATION` in one source file before including the header

3. **Implementation Details:**
   - Regular version: Implementation is in separate source files
   - Single-header version: Implementation is included in the header when `BETTER_STRING_IMPLEMENTATION` is defined 