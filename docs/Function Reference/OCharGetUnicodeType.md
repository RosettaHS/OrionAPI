# Function Reference: OCharGetUnicodeType
## Encoding Evaluation
Analyses the [UTF-8 header](https://en.wikipedia.org/wiki/UTF-8#Encoding) of the given character and returns the corrisponding Unicode Header type

## Declaration
```cpp
extern OUnicodeType OCharGetUnicodeType(char eval);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : src/OSL/OString.cpp]
```

### Parameters
`char eval` - The character to evaluate.

### Returns
One of the many possible enumerations included in [OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md).
Review the documentation for [OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md) for further details.

## Use
OCharGetUnicodeType is used for determining the UTF-8 [encoding state](https://en.wikipedia.org/wiki/UTF-8#Encoding) of a given character byte.

**\[This assumes you have read the documentation for [OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md). Please review this and return here if you have not seen it already\]**

To retrieve a corresponding OUnicodeType from a byte, use the following syntax:
```cpp
char myByte='A';
OUnicodeType myState=OCharGetUnicodeType(myByte);
```
In this example, `myState` will get the value `OUNI_ASCII`, since `'A'` is a [standard ASCII character](https://en.wikipedia.org/wiki/ASCII),
not requiring any UTF-8 continuation bytes.

The return value