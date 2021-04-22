# Function Reference: OLog
## Orion-Native Logging

## Declaration
```cpp
extern void OLog(const char* string, ...);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OLog.hpp]
[Defined  : src/OSL/OLog.cpp]
```

### Parameters
`const char* string` - The String to be formatted and logged. The rest of the arguments can be of any type as long as they're capable of being [formatted](https://en.wikipedia.org/wiki/Printf_format_string) into the String.

`...` [Variadic](https://en.wikipedia.org/wiki/Variadic_function) arguments. Can be of any type as long as long as they're capable of being [formatted](https://en.wikipedia.org/wiki/Printf_format_string) into the String.

## Use

