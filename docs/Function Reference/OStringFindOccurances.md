# Function Reference: OStringFindOccurances
## Occurance Checking
Returns the count of times the substring is found in the given String.

## Declaration
```cpp
extern size_t OStringFindOccurances(const char* string, const char* substring);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : src/OSL/OString.cpp]
```

### Parameters
`const char* string` - The String to attempt to search the substring in.

`const char* substring` - The substring to search for in the given String.

### Returns
The number of times the substring was found in the given String.
