# Type Reference: OString
## Orion-Native Unicode String handling
OString is the Orion-Native implementation for handling Unicode Strings with [variable length](https://en.wikipedia.org/wiki/UTF-8) characters.

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
**\[This section assumes you have read the documentation for [OChar](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OChar.md) \]**