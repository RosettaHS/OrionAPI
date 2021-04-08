# Type Reference: OUnicodeType
## Classifying UTF-8 Characters
OUnicodeType is an [enumeration](https://en.wikipedia.org/wiki/Enumerated_type) of all possible [encoding states](https://en.wikipedia.org/wiki/UTF-8#Encoding)
of a given [UTF-8](https://en.wikipedia.org/wiki/UTF-8) byte.

## Expansion
```cpp
enum OUnicodeType : char{
	OUNI_NULL=-1,
	OUNI_ASCII,
	OUNI_CONTINUE,
	OUNI_HEADER_1X,
	OUNI_HEADER_2X,
	OUNI_HEADER_3X,
	OUNI_HEADER_4X,
	OUNI_HEADER_5X,
	OUNI_UNKNOWN
};
```
```
[Altname  : unitype_t]
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : <none>]
```

## Use
OUnicodeType is used to determine the encoding state of a given character. This is often retrieved from a function such as  `OCharGetUnicodeType()`,
or used internally in `OChar`.

To retrieve a corresponding OUnicodeType from a byte, use the following syntax:
```cpp
char myByte='A';
OUnicodeType myState=OCharGetUnicodeType(myByte);
```
In this example, `myState` will get the value `OUNI_ASCII`, since `'A'` is a [standard ASCII character,](https://en.wikipedia.org/wiki/ASCII)
not requiring any UTF-8 continuation bytes.

### Multi-byte Unicode Characters
OrionAPI provides a correct way to handle multi-byte characters with `OChar`, however this example will focus on handling the individual bytes directly.

In this example, we will be using the Grape Emoji: 🍇

The Grape Emoji may appear to be one character; one byte, however examining the raw data will reveal it is actually a multi-byte character:
```
[11110000], [10011111], [10001101], [10000111]
  Byte 1      Byte 2      Byte 3      Byte 4  
```
When examining the raw data of the letter `A`, we will find it is only one byte, but another thing can also be observed:
```
[01000001]
  Byte 1  
```
Looking closer at the bits near the start (left-most bits) of each byte, a pattern emerges.

For all ASCII (single-byte) characters, the left-most bit is always `0`.
However for the left-most bit of all of the Grape's bytes, it is always `1`.

If a character byte contains a `1` as the left-most bit, it is a flag that states that the bit is a multi-byte character.
Examining the bits immediately after will also give extra clues about the character and bytes themselves.

Let's take a closer look at the first byte of the Grape Emoji:
```
[11110000]--------+
  Byte 1          |
                  |
 [One One One One | Zero Zero Zero Zero]
```