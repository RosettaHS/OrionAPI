# Function Reference: OCharGetUnicodeType
## Encoding Evaluation
Analyses the [UTF-8 header](https://en.wikipedia.org/wiki/UTF-8#Encoding) of the given character and returns the corrisponding Unicode Header type

## Declaration
```cpp
extern OUnicodeType OCharGetUnicodeType(unsigned char eval);
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

To retrieve a corresponding [OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md) from a byte, use the following syntax:
```cpp
char myByte='A';
OUnicodeType myState=OCharGetUnicodeType(myByte);
```
In this example, `myState` will get the value `OUNI_ASCII`, since `'A'` is a [standard ASCII character](https://en.wikipedia.org/wiki/ASCII),
not requiring any UTF-8 continuation bytes.

The return value can be one of the following states:
```
OUNI_NULL      - The byte is comprised entirely of zeros; a NULL terminator.
OUNI_ASCII     - The byte is ASCII, and should not be treated as Unicode.
OUNI_CONTINUE  - The byte is a UTF-8 continuation byte.
OUNI_HEADER_1X - The byte is a UTF-8 header byte that calls for one extra continuation byte.
OUNI_HEADER_2X - The byte is a UTF-8 header byte that calls for two extra continuation bytes.
OUNI_HEADER_3X - The byte is a UTF-8 header byte that calls for three extra continuation bytes.
OUNI_HEADER_4X - The byte is a UTF-8 header byte that calls for four extra continuation bytes.
OUNI_HEADER_5X - The byte is a UTF-8 header byte that calls for five extra continuation bytes.
OUNI_UNKNOWN   - The byte is of an unknown type.
```

As an example, the following is the binary information of the multi-byte [Emoji](https://en.wikipedia.org/wiki/Emoji) `🍇`:
```
 [11110000], [10011111], [10001101], [10000111]
   Byte 1      Byte 2      Byte 3      Byte 4  
```

Running each byte through OCharGetUnicodeType will return the following [OUnicodeTypes](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md):
```
[11110000], [10011111], [10001101], [10000111]
  Byte 1      Byte 2      Byte 3      Byte 4  
    |           |           |           |
    |           |           |          OUNI_CONTINUE
    |           |          OUNI_CONTINUE
    |          OUNI_CONTINUE
   OUNI_HEADER_3X
```

## Other Information
OCharGetUnicodeType is only meant to be used on bytes corrisponding to a valid character. Attempting to send invalid binary information (such as bytes not belonging to a String or character)
will return incorrect results. Just because OCharGetUnicodeType can return a valid [OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md),
it doesn't mean that the byte should be treated as a valid Unicode byte.

#### See Also
[OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md)