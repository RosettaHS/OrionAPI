# Function Reference: OStringCompare
## String Comparison
Compares the two strings and returns true if they share identical bytes.

## Declaration
```cpp
extern bool OStringCompare(const char*, const char*);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : src/OSL/OString.cpp]
```

#### Returns
`True` if both Strings are identical, `false` if there are any variations.

## Use
OStringCompare is used for checking if two Strings have the same content.

To use OStringCompare, use the following syntax:
```cpp
const char* stringA="Hello!";
const char* stringB="Hello!";

bool myResult=OStringCompare(stringA,stringB);
```
In this example, `myResult` will be `true`, since both Strings share the same content (bytes).

If there are any variations whatsoever, the result will **always** be `false`:
```cpp
const char* stringA="Hello!";
const char* stringB="Hallo!";

bool myResult=OStringCompare(stringA,stringB);
```
The result will be `false` since despite `Hello!` and `Hallo!` being incredibly similar, they are still different, therefore not identical.

## Other Information
Beware using this on Strings with [multi-byte](https://en.wikipedia.org/wiki/UTF-8) characters, as they have [a chance of not sharing identical bytes despite appearing visually identical](https://youtu.be/tOHnXt3Ycfo?t=2658).

#### See Also:
[OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)

[OStringFindFirst](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindFirst.md)

[OStringFindLast](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindLast.md)

[OStringFindOccurances](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindOccurances.md)

[OStringLength](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringLength.md)