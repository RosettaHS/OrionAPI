# Type Reference: OString
## Orion-Native Unicode String handling
OString is the Orion-Native implementation for handling Unicode Strings with [UTF-8 encoded](https://en.wikipedia.org/wiki/UTF-8) characters.

## Expansion
```cpp
class OString : public CLoggable{
	protected:
		char*  raw;
		struct{
			uint32_t apparent;
			uint32_t real;
		}length;
		uint32_t memuse;

		uint32_t apparentToReal(uint32_t index);
	public:
		~OString(void);
		inline OString(void) : raw{0}, length{0,0}, memuse{0} {};
		inline OString(const char* text) : raw{0}, length{0,0}, memuse{0} { setTo(text); };

		bool clear(void);

		bool setMemory(uint32_t newSize);

		bool setTo(const char* text); OString& operator=(const char* text);
		bool append(const char* text); OString& operator+=(const char* text);
		bool setChar(uint32_t index, OChar c);

		char* getText(void) const; operator char*(void) const;
		OChar getChar(uint32_t index); OChar operator[](uint32_t);
		inline char getCharFast(uint32_t index) { return raw[index]; }
		uint32_t getLength(bool realLength=false);
		inline uint32_t getMemory(void) { return memuse; }

		bool equalTo(const char* text) const; bool operator==(const char* text) const;
		bool ready(void) const; operator bool(void) const;
		bool contains(const char* substring);

		OString operator+(const char* text) const;

		virtual void log(bool verbose=false,  bool newLine=true) override;
};
```
```
[Altname  : string_t]
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : src/OSL/OString.cpp]
```

## Use
OString is meant to be the primary way of handling Strings using OrionAPI. It provides many utilities for modifying Strings alongside full support for multi-byte characters.

To create an OString for use, use the following syntax:
```cpp
OString myString="Hello";
```
This will copy the contents of the [string literal](https://en.wikipedia.org/wiki/String_literal) to the new OString.

OStrings can be passed into any functions that take in a `char*` or `const char*`.

OStrings also allow modification, such as [concatenation,](https://en.wikipedia.org/wiki/Concatenation) and setting individual characters.
The following code will add extra text to the end of a String:
```cpp
OString myString="Hello";
myString+=" World!";

myString.log(); /* To print the String. */
```
This will add `" World!"` to the end of `"Hello"` and log it out to the terminal. The output will be `Hello World!`

Changing individual characters of a String is also possible.
The following code will change a single character in the String `"Hello"`:
```cpp
/* In this String, "e" is at index 1. */
OString myString="Hello";
myString.setChar(1,'a');

myString.log(); /* To print the String. */
```
This will change the letter `'e'` to `'a'`. The output will be `Hallo`.

### Unicode Support
**\[This section assumes you have read the documentation for [OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md) and [OChar.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md) If you have not read them, it is recommended you read them now before returning to this\]**

Much like [OChar,](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md) OString is fairly useless on its own.
However, the main purpose behind OString is to easily handle Strings with [variable-width characters.](https://en.wikipedia.org/wiki/Variable-width_encoding)

This is described in full in the documentation for [OUnicodeType.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md)
For now, this will supply a brief explanation.

To understand how and why OString handling Unicode Strings is important, take the following example:

Take the String `"Hi 👋"`.
The String may appear to be comprised of 4(+1) characters:
```
[H] [i] [ ] [👋] [0]
 |   |   |   |    |
 1   2   3   4    5 (NULL TERMINATOR)
```
However, examining the bytes we will find it is actually comprised of 7(+1) characters(bytes):
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
This makes reading and writing to Strings with variable-length characters incredibly unintuitive and complicated.

Using the example string `"Hi 👋"`, attempting to directly index and replace `"👋"` with another character of unequal size such as `"!"` will cause severe issues and result in a malformed String:
```
[H] [i] [ ] [!] [0]
 |   |   |   |   |
 |   |   |   |   +-----------------------------------------------------------------------+
 |   |   |   |                                                                           |
 |   |   |   +---------------------------+            ?          ?           ?           |
 |   |   |                               |            |          |           |           |
 |   |   +-------------------+           |            |          |           |           |
 |   |                       |           |            |          |           |           |
 |   +-----------+           |           |            |          |           |           |
 |               |           |           |            |          |           |           |
 +---+           |           |           |            |          |           |           |
     | #1        | #2        | #3        | #4         | #5       | #6        | #7        | #8
 [01001000], [01101001], [00100000], [00100001], [10011111], [10010001], [10001011], [00000000]
 [  "H"   ]  [  "i"   ]  [  " "   ]  [  "!"   ]  [  ????  ]  [  ????  ]  [  ????  ]  [  NULL  ]
```
The result is a severely malformed String: `Hi !���`.

The difficulty and errors will only increase when attempting to modify larger Strings with more multi-byte characters at different locations.
It is also incredibly difficult to gauge the actual size of a String when it has multi-byte characters.

Take this String for example: `"I 💜 grapes 🍇 😋"`.
The apparent character count is 14(+1), however the raw byte count is 23(+1):
```
[I] [ ] [💜] [ ] [g] [r] [a] [p] [e] [s] [ ] [🍇] [ ] [😋] [0]
|-------------------------BYTE COUNT-------------------------|
[1] [1] [4B] [1] [1] [1] [1] [1] [1] [1] [1] [4B] [1] [4B] [1] (NULL)
```
#### OString's Abstraction
The way OString makes all of this easy, is instead of making developers index and write to individual bytes of a given String,
OString instead handles all operations based on the apparent characters.

Going back to the example string `"I 💜 grapes 🍇 😋"`,
the apparent position(index) of the letter `"g"` at the start of the word `"grapes"` is at index `4`:
```
[I 💜 g]---+
           | 
 [I] [ ] [💜] [ ] [g]
  0   1   2    3   4
```
However, in reality, its real index is at position `7`:
```
[I 💜 g]---+
           |
 [I] [ ] [💜] [ ] [g]
  |   |   |    |   |
  |   |   |    |   +----------------------------------------------------------------------+
  |   |   |    |                                                                          |
  |   |   |    +--------------------------------------------------------------+           |
  |   |   |                                                                   |           |
  |   |   +-------------------+-----------+-----------+-----------+           |           |
  |   |                       |           |           |           |           |           |
  |   +-----------+           |           |           |           |           |           |
  |               |           |           |           |           |           |           |
  +---+           |           |           |           |           |           |           |
      |           |           |           |           |           |           |           |
      | #0        | #1        | #2        | #3        | #4        | #5        | #6        | #7
  [01001001], [00100000], [11110000], [10011111], [10010010], [10011100], [00100000], [01100111]
  [  "I"   ]  [  " "   ]  [                    "💜"                    ]  [  " "   ]  [  "g"   ]
```
Therefore, retrieving `"g"` using a traditional character array(String) is done like this:
```cpp
const char* myString="I 💜 grapes 🍇 😋";
char myChar=myString[7];

OLog("%c\n",myChar); /* Print the retrieved character. */
```
The output will be `g`.

However, doing the same with OString, we can index using apparent positions instead of directly accessing the memory:
```cpp
OString myString="I 💜 grapes 🍇 😋";
char myChar=myString[4];

OLog("%c\n",myChar); /* Print the retrieved character. */
```
The output will also be `g`, despite indexxing at a different position.

All operations with OString are done with apparent indices, making it incredibly easy to access and modify a given String.
For example: the following code replaces the `"🍇"` at apparent index `11` to a different character of a different byte size,
without disrupting the String itself:
```cpp
OString myString="I 💜 grapes 🍇 😋";
myString.setChar(11,'!');

myString.log(); /* To print the String. */
```
The output will be `I 💜 grapes ! 😋`.

The apparent length of the String stays the same after the operation, however the byte count changes, and the excess bytes from the removed emoji are trimmed, shifting the rest of the String over by the excess.

Retrieving multi-byte characters is just as easy when using [OChar.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md)
The following code retrieves the `"💜"` at apparent index `2` and prints it out to the terminal:
```cpp
OString myString="I 💜 grapes 🍇 😋";
OChar myChar=myString[2];

myChar.log(); /* To print the Char. */
```
The output will be `💜`.

### When to use OString
OStrings should be used whenever interacting with user generated text, retrieving text from a File, or when doing modification (such as concatenation) on any text in general.

It should not be used in situations where memory is scarce or processor operations should be kept to a minimum,
as OStrings use more memory than traditional character arrays, and reading/writing to OStrings uses many internal operations when not using the direct methods.

## Breakdown
### The following members are protected, and cannot be accessed directly.
```cpp
char*  raw;
```
The actual String stored in memory.
```cpp
struct{
	uint32_t apparent;
	uint32_t real;
}length;
```
A struct containing two different interpretations of this String's length.

`uint32_t apparent;` The apparent length of this String (without Unicode continuation bytes.)

`uint32_t real;` The real length of this String (with all non-null bytes.)

```cpp
uint32_t memuse;
```
The actual memory being used by this String (including null terminator.)
```cpp
uint32_t apparentToReal(uint32_t index);
```
Converts an apparent index (as is inputted in setChar and getChar) to a real index used for computation.

Parameters:

`uint32_t index - The given apparent index to convert.`

Returns: `A real index that can be used directly on the internal String pointer for operations.`
### The following members are public, and can be accessed directly.
```cpp
~OString(void);
```
Destructor. Frees all memory.
```cpp
inline OString(void) : raw{0}, length{0,0}, memuse{0} {};
```
Empty constructor. Sets all values to 0.
```cpp
inline OString(const char* text) : raw{0}, length{0,0}, memuse{0} { setTo(text); };
```
Initialises this String with the given text.

Parameters:

`const char* text - The text for this String to copy and use.`
```cpp
bool clear(void);
```
Frees all memory stored by this String.

Returns: `True if there was memory to free, false if could not free memory, or memory was non-existent. `

```cpp
bool setMemory(uint32_t newSize);
```
Sets the exact memory (including terminator) to the given value. Truncates String if the new memory is smaller than the length.

Parameters:

`uint32_t newSize - The exact size (in bytes) that this String will use, including the terminator.`

Returns: `True if memory could be changed, false if either new memory size is identical or otherwise simply could not be resized.`
```cpp
bool setTo(const char* text);
OString& operator=(const char* text);
```
Sets this String to the given text.

Parameters:

`const char* text - The text for this String to copy and use.`

Returns: `True if String could be set, false if memory for the new String coould not be allocated.`
```cpp
bool append(const char* text);
OString& operator+=(const char* text);
```
Appends the given text to the end of this String.

Parameters:

`const char* text - The text to append to this String.`

Returns: `True if text could be appended, false if memory for the new String coould not be allocated.`
```cpp
bool setChar(uint32_t index, OChar c);
```
Sets the character at the given index.

Parameters:

`uint32_t index - The apparent index at which to set the new character to.`

`OChar c - The single/multi-byte character to set.`

Returns: `True if new character could be set, false if either the String has not been initialised, or the index is out of bounds.`
```cpp
inline void setCharFast(uint32_t index, char c) { raw[index]=c; }
```
Same as setChar() but directly sets the character in memory. Quicker, but loses easy Unicode helpers.

Parameters:

`uint32_t index - The real index at which to set the new character to.`

`char c - The single-byte character to set.`
```cpp
char* getText(void) const;
operator char*(void) const;
```
Returns: `A pointer to the character array used by this String.`
```cpp
OChar getChar(uint32_t index);
OChar operator[](uint32_t);
```
Gets and returns the Unicode character at the given index.
Indexing operations are done based on apparent length, not actual bytes.

Parameters:

`uint32_t index - The apparent index to locate the character at.`

Returns: `An OChar, abstraction for single or multi-byte characters. See the documentation for OChar.`
```cpp
inline char getCharFast(uint32_t index) { return raw[index]; }
```
Same as getChar() but directly indexes the String. Quicker, but loses easy Unicode helpers.

Parameters:

`uint32_t index - The real index to locate the character at.`

Returns: `The single-byte character found at the index.`
```cpp
uint32_t getLength(bool realLength=false);
```
Returns the length of this String.

Parameters:

`bool realLength - Should this function return the actual length of this String, including the Unicode continuation bytes? Default is false.`

Returns: `The length of this String dependent on the parameter used on this method.`
```cpp
inline uint32_t getMemory(void) { return memuse; }
```
Returns: `The raw memory usage of this String, including any unused bytes. Unused bytes are kept in case any concatenation is needed.`
```cpp
bool equalTo(const char* text) const;
bool operator==(const char* text) const;
```
Is this String equal to the given String?

Parameters:

`const char* text - The separate String to compare.`

Returns: `True if both Strings are identical, false if there are any variations.`
```cpp
bool ready(void) const;
operator bool(void) const;
```
Is this String ready for use / does it point to valid memory?

Returns:`If the String's internal character array pointer is set, this will return true. Otherwise it will return false, meaning the String has not been initialised.`
```cpp
bool contains(const char* substring);
```
Does this String contain the given substring?

Parameters:

`const char* substring - The substring to attempt to find within this String.`

Returns: `True if substring could be found, false if it could not.`
```cpp
OString operator+(const char* text) const;
```
Creates a new String that is the combination of this String and the given text.

Parameters:

`const char* text - The String to append to the new String`

Returns: `A new, separate String with the given text appended to the end.`
```cpp
virtual void log(bool verbose=false, bool newLine=true) override;
```
Logs this String out to the terminal.

Parameters:
`bool verbose - Log verbose information (such as memory usage and alongside the String itself. Default is false.`
`bool newLine - Should the output be placed on a newline or append to the current one if applicable? Default is true.`

## Other Information
Much like [OChar,](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md)
when inputting an OString as a parameter into a [variadic function,](https://en.wikipedia.org/wiki/Variadic_function) such as `OLog()`,
you should *always* send the contents of the OString instead of the OString itself:
```cpp
OString myString="Hello World!";
OLog("%s\n",myString.getText()); /* Do NOT just put in myString! */
```

#### See Also:
[OChar](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md)

[OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md)