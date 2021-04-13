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
myString.setChar('a',1);

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