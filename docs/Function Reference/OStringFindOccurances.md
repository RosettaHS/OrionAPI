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

## Use
OStringFindOccurances is used for counting how many times a substring is found within a String
Note that it does **NOT** return the indicies of those substrings, only the count of substrings found.

Take the example String `"I love Grapes. Grapes are delicious"`

`**TEST** TEST`