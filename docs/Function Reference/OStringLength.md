# Function Reference: OStringLength
## Length retrieval of Unicode Strings
Writes the formatted String to the output.

## Declaration
```cpp
extern size_t OStringLength(const char* string, bool getApparentLength=false);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : src/OSL/OString.cpp]
```

### Parameters
`const char* string` - The String to get the length of.

`bool getApparentLength` - Should this function return the raw length (in bytes) of the String,
or the String's apparent length (excluding UTF-8 continuation bytes)? Default is false.

### Returns
The length of the String, dependent on the mode specificed by the second parameter.

## Use
OStringLength is used for retrieving the length(in characters) of a String up until the [null terminator](https://en.wikipedia.org/wiki/Null-terminated_string).
It provides two different ways to retrieve the length.

### Objective Length
By default, when attempting to retrieve the length of a String, it will return the exact number of bytes of the String (except for the terminator):
```cpp
size_t myLength=OStringLength("Hello");
```
The result will be `5`, since `"Hello"` contains 5 bytes.

However, this method of determining String length is flawed, as [multi-byte characters](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md#multi-byte-unicode-characters)
are counted incorrectly.

### Apparent Length
Instead of counting the number of bytes in a String to determine its length, this method instead counts the number of individual characters

When using it on normal text, the outcome doesn't change:
```cpp
size_t myLength=OStringLength("Hello",true);
```
The result will still be `5`, since `"Hello"` contains 5 characters.

Where the result does change is when inputting Unicode Strings.

Take the following String as an example: `"Hi 👋"`. It appears to be 4 characters long, however running it through OStringLength normally will return a different result:
```cpp
size_t myLength=OStringLength("Hi 👋");
```
The result will actually be `7` instead of `4`.

This is because the 4th character `👋` is actually multi-byte:
```
[H] [i] [ ] [👋] [0]
 |   |   |   |    |
 |   |   |   |    +----------------------------------------------------------------------+
 |   |   |   |                                                                           |
 |   |   |   +---------------------------+------------+----------+-----------+           |
 |   |   |                               |            |          |           |           |
 |   |   +-------------------+           |            |          |           |           |
 |   |                       |           |            |          |           |           |
 |   +-----------+           |           |            |          |           |           |
 |               |           |           |            |          |           |           |
 +---+           |           |           |            |          |           |           |
     | #1        | #2        | #3        | #4         | #5       | #6        | #7        | #8
 [01001000], [01101001], [00100000], [11110000], [10011111], [10010001], [10001011], [00000000]
 [  "H"   ]  [  "i"   ]  [  " "   ]  [                   "👋"                     ]  [  NULL  ]
```
More information on why this is the case can be found in the documentation for [OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md), [OChar](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md), and [OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md#unicode-support).

When passing `true` as the second argument to OStringLength, it will ignore counting the bytes of multi-byte characters and only focuses on the headers,
effectively counting the amount of characters instead of the amount of bytes.

Now, OStringLength functions as expected:
```cpp
size_t myLength=OStringLength("Hi 👋",true);
```
The result will be `4`.

### Benfits to both
Both options are supplied for a reason. They have their own use cases, using one over the other is entirely dependent on the situation.

#### Counting Objectively
**Benefits:**

* Essentially required for indexxing operations
* Correctly returns the exact memory usage of any String, minus one (for the terminator)
* More efficient to count individual bytes as opposed to checking if the byte is a valid character header

**Drawbacks:**

* Displaying the character count of/in a textbox of user interface will appear incorrect and lead to user frustration

#### Counting Apparently
**Benefits:**

* Essentially required for displaying character counts in a user interface

**Drawbacks:**

* More computationally intensive than directly counting individual bytes
* Cannot be safely used in a `for()` loop when indexxing a String
* Does not safely return the memory usage of a String

## Other Information
Despite appearent counting having more drawbacks, it is still the preferred solution for displaying character counts to a user for graphical Applications, or any form of text entry.
As stated previously, the usage of either option is entirely dependent on the situation. There is no absolute best choice.

#### See Also:
[OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md)

[OChar](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md)

[OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)

[OStringCompare](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringCompare.md)

[OStringFindFirst](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindFirst.md)

[OStringFindLast](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindLast.md)

[OStringFindOccurances](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindOccurances.md)