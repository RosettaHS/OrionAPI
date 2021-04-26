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
OUnicodeType is used to determine the encoding state of a given character. This is often retrieved from a function such as [OCharGetUnicodeType()](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OCharGetUnicodeType.md),
or used internally in [OChar](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md).

To retrieve a corresponding OUnicodeType from a byte, use the following syntax:
```cpp
char myByte='A';
OUnicodeType myState=OCharGetUnicodeType(myByte);
```
In this example, `myState` will get the value `OUNI_ASCII`, since `'A'` is a [standard ASCII character](https://en.wikipedia.org/wiki/ASCII),
not requiring any UTF-8 continuation bytes.

### Multi-byte Unicode Characters
OrionAPI provides a correct way to handle multi-byte characters with [OChar](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md). however this example will focus on handling the individual bytes directly.

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

If a character byte contains a `1` as the left-most bit, the `1` is a flag that states that the byte is part of a multi-byte character.
Examining the bits immediately after will also give extra clues about the character and bytes themselves.

Let's take a closer look at the first byte of the Grape Emoji:
```
[11110000]--------+
  Byte 1          |
                  |
 [One One One One | Zero Zero Zero Zero]
   |   |   |   |     |    |     |    |
   |   |   |   |     |    +-----+----+---+
   |   |   |   |     END OF FLAGS        |
   |   |   |  +3 CB Total          [ Byte Data ] (Empty in this case)
   |   |  +2 CB Total
   |  Calls for +1 Continuation Bytes (CB)
 IsMultibyte = 1 (True)
```
Examining the bits, we find the first byte of the Grape Emoji declares itself as multi-byte (first bit),
and then the next three bits are active, which indicates that this byte is the header byte of a multi-byte character that spans three bytes (calling for three continuation bytes).

After the first three bits, a `0` is encountered. The first `0` always denotes the end of any UTF-8 flags. The remaining bits contain data regarding the byte,
which in this case, is three zeros.

Examining the second byte, more information can be gathered:
```
[10011111]----------+
  Byte 2            |
                    |
 [One Zero Zero One | One One One One]
   |   |    |    |     |   |   |   |
   |   |    +----+-----+---+-+-+---+
   |   END OF FLAGS          |
   |                   [ Byte Data ]
 IsMultibyte = 1 (True)
```
This is true for all of the other bytes except for the first (header) byte, the only difference being the byte data section changing.

Whereas the header byte had the four left-most bits on, the rest of the bytes have only one of the left-most bits on.
This is to mark that they are not header bytes, but rather continuation bytes.

A header byte declares how many continuation bytes there will be, and each continuation byte contains a flag to indicate that they are indeed continuation bytes.
They contain the rest of the data required to store the character.

Running each byte through `OCharGetUnicodeType()` will return the following OUnicodeTypes:
```
[11110000], [10011111], [10001101], [10000111]
  Byte 1      Byte 2      Byte 3      Byte 4  
    |           |           |           |
    |           |           |          OUNI_CONTINUE
    |           |          OUNI_CONTINUE
    |          OUNI_CONTINUE
   OUNI_HEADER_3X
```
### Internal Operation
This is entirely abstracted by OrionAPI through the [OCharGetUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OCharGetUnicodeType.md) function, but this is a description of how identifying Unicode bytes may be done.

A `1` as the left-most bit will **always** indicate that the given byte is indeed a Unicode byte.
If this bit is true, any and all `1`s after the left-most bit will count the number of continuation bytes required for the given character.
Technically, following this approach, a maximum of five continuation bytes are possible:
```
[11111101]-------+
  Byte 1         |
                 |
 [One One One One One One Zero One]
   |   |   |   |   |   |   |    |
   |   |   |   |   |   |   |    +-----+
   |   |   |   |   |   |   |          |
   |   |   |   |   |   |   |    [ Byte Data ] (Can be 1 or 0 since it's just one bit)
   |   |   |   |   |   |   END OF FLAGS
   |   |   |   |   |  +5 CB Total
   |   |   |   |  +4 CB Total
   |   |   |  +3 CB Total
   |   |  +2 CB Total
   |  +1 CB Total
  IsMultibyte = 1 (True)
```
Although, it's important to know that the UTF-8 standard itself ONLY allows a maximum of 4 continuation bytes.

When counting each true bit starting from the left, the counting should immediately stop when a false bit (`0`) is encountered, as this signals the end of the UTF-8 bit flags.
The remaining bits after the false bit contain generic byte data.

The count of true bits, subtracted by 1, will give the number of continuation bytes that the header calls for.

If the left-most bit is the only true bit before a `0`, it indicates that the byte is not a header, but a continuation byte.

If the left-most bit is `0`, this means the byte is an ASCII single-byte character, and should not be read as Unicode.

Interestingly, by counting each true bit (`1`) up until a false bit (`0`) is found, a number (from 0-6) can be generated which can be mapped directly to an enumeration as is done with OUnicodeType
```
OUNI_NULL      : -1 - Uninitialised, NULL byte.
OUNI_ASCII     :  0 - Immediate false bit. Is ASCII
OUNI_CONTINUE  :  1 - Left-most bit is true, next bit is false. Is a Continuation byte.
OUNI_HEADER_1X :  2 - Two left-most bits are true. Is a Header (that calls for one extra CB)
OUNI_HEADER_2X :  3 - Three left-most bits are true. Is a Header (that calls for two extra CBs)
OUNI_HEADER_3X :  4 - Four left-most bits are true. Is a Header (that calls for three extra CBs)
OUNI_HEADER_4X :  5 - Five left-most bits are true. Is a Header (that calls for four extra CBs)
OUNI_HEADER_5X :  6 - Six left-most bits are true. Is a Header (that calls for five extra CBs)
OUNI_UNKNOWN   :  7 - Seven left-most bits are true. This isn't a valid header byte, therefore type is unknown.
```
This is how [OCharGetUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OCharGetUnicodeType.md) works internally, merely counting the true bits up until a false bit, then mapping that directly to the enumeration and returning. 
## Other Information
This is merely meant to be an informative piece. Working with raw UTF-8 bytes/bits isn't recommended.
OrionAPI provides a minimal abstraction for individual Unicode characters ([OChar](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md)), and an abstraction for UTF-8-compliant Strings ([OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)).

Both of those classes automatically handle multi-byte Unicode characters natively and completely transparently,
making Unicode Strings as simple to read and modify as standard ASCII Strings. Review the documentation for both of those classes for more information.

#### See Also:
[OChar](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md)

[OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)