# Type Reference: OChar
## Multi-byte Unicode Handling
OChar is the Orion-Native implementation for handling multi-byte Unicode([UTF-8](https://en.wikipedia.org/wiki/UTF-8)) characters.

## Expansion
```cpp
struct OChar : CLoggable{
	union{
		char          asSingleByte;
		unsigned char asMultiByte[5];
	}get;
	OUnicodeType  bd[5];
	unsigned char byteCount;
	bool          isUnicode;

	inline OChar(void) : get{0}, bd{OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL}, byteCount{1}, isUnicode{0} {}
	OChar(const char*); void setTo(const char*); OChar& operator=(const char*);
	OChar(char); void setTo(char); OChar& operator=(char);

	void clear(void);

	inline operator char(void)        { return get.asSingleByte; }
	inline operator const char*(void) { return ( (isUnicode) ? (const char*)get.asMultiByte : 0 ); }

	virtual void log(bool verbose=false, bool newLine=true) override;
};
```
```
[Altname  : unichar_t]
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : src/OSL/OString.cpp]
```

## Use
OChar is used to store, read, and insert a multi-byte Unicode character, also providing utilities to examine the state and details of the character itself.

Internally, OChar is a [null-terminated](https://en.wikipedia.org/wiki/Null_character) String with a fixed length of 4(+1 for terminator),
the maximum length for a multi-byte UTF-8 character as described by the standard.

**NOTE:** Do **NOT** use OChar as a replacement for `char` when making Strings(character arrays). **ONLY** use OChar for retrieving or setting individual characters as is done with [OString.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)

An OChar can be initialised from either a single-byte character or a character array(String).
To initialise an OChar for use, use the following syntax:
```cpp
OChar myChar='A'; /* Can also be done using "A" instead of 'A' */
```
In this example, `myChar` is being set to an [ASCII character.](https://en.wikipedia.org/wiki/ASCII)
It can then be examined and placed in something that stores an ASCII character:
```cpp
char myByte;
/* First check if it isn't Unicode. This could be skipped in this example since it was set to a known ASCII character. */
if(myChar.isUnicode==false){ /* 'A' is ASCII, not Unicode. Will be false. */
	myByte=myChar; /* Could also do myChar.get.asSingleByte */
}
```
Being able to store single-byte characters doesn't require OChar, however storing and reading multi-byte characters efficiently does.
The following code stores a multi-byte Unicode character (🍇) and initialises all internal information:
```cpp
OChar myChar="🍇";
```
Multi-byte characters have to come in form of a String. The contents are evaluated and stored within OChar.

Putting back a multi-byte character into a primitive String can be done with the following syntax:
```cpp
const char* myMultiByte;
/* First check if it is Unicode, otherwise this will fail. */
if(myChar.isUnicode){ /* "🍇" is Unicode. Will be true. */
	myMultiByte=myChar; /* Could also do myChar.get.asMultiByte */
}
```
Note that this does not copy the contents of the OChar, it only merely changes the pointer `myMultiByte`.

### OString Integration
OChar is fairly useless on its own, the previous examples could all be done with character arrays and manual examination.
However, OChar's usefulness shines when paired with [OString,](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) the Orion-Native implementation for Unicode Strings.

Information on how [OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) is used, or how it works can be found within [the documentation for OString.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)
This example will merely explain how
OChar can be used alongside [OString.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)

Traditionally, indexing multi-byte Unicode characters stored within a character array(String) is notoriously difficult.

Attempting to retrieve and store an emoji (with a known place) from a String without using [OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) or OChar is extremely difficult,
however if we use some tools (mainly [OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md) and `OCharGetUnicodeType()`),
it could be done like this:
```cpp
/* In this String, the "🍇" emoji starts at position 7. */
const char* myString="Grapes 🍇 are delicious!";
size_t      stringLength=OStringLength(myString);
size_t      grapePos=7;    /* Known position. The difficulty with an unknown position would be monsterous. */
char        grape[5];      /* This is where the "🍇" emoji will be stored after extraction. Can safely assume size of at most 4(+1) bytes due to UTF-8. */
size_t      grapeLength=0; /* We don't know how many bytes the "🍇" emoji is, we need to figure this out first and store it here. */

/*
 * We will use OUnicodeType and OCharGetUnicodeType() to examine the UTF-8 header of the "🍇" emoji.
 * Doing this manually would be even more monsterous.
 */
OUnicodeType grapeHeader=OCharGetUnicodeType(myString[grapePos]);
if(grapeHeader>0){
	grapeLength=grapeHeader; /* To understand why this works, review documentation for OUnicodeType. */
}
size_t j=0; /* Secondary Counter. */
for(size_t i=grapePos;i<stringLength;i++){
	if(j==grapeLength){ break; }
	else{
		grape[j]=myString[i];
		j++;
	}
}
grape[4]=0; /* NULL terminator. */

OLog("%s\n",grape); /* Prints out the extracted emoji. */
```
This is incredibly difficult, and this approach completely falls apart with unknown character positions and malformed Strings,
it's also incredibly difficult to attempt to index characters *after* the multi-byte character.

However, utilising the Orion-Native [OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) and OChar, it could be done as easily as this:
```cpp
/* In this String, the "🍇" emoji exists entirely at position 7. */
OString myString="Grapes 🍇 are delicious!";
OChar   grape=myString[7];

grape.log(); /* Prints out the extracted emoji. */
```
Even more impressively, [OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) handles character indexing by apparent characters, not individual bytes.
As an example, retrieving the letter "a" at the start of the word "are" after "🍇" in the String can be done like this:
```cpp
/* In this String, the "a" at the start of "are" starts at position 9. */
OString myString="Grapes 🍇 are delicious!";
OChar   grape=myString[9]; /* "🍇" at [7], " " at [8], and "a" at [9]. */

grape.log(); /* Prints out the extracted letter. */
```
Doing this without OString and OChar is incredibly difficult due to having to convert apparent indices to real, byte indices (since not all characters are single-byte).

Setting a multi-byte character within a traditional String is even more complicated, however OChar and [OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) make this also incredibly easy.

In this example, the letters of each word are replaced with "🍇" except for the spaces:
```cpp
OString myString="Grapes 🍇 are delicious!";
OChar   grape=myString[7];
for(size_t i=0;i<myString.getLength();i++){
	if(myString[i]!=' '){ /* If it isn't a space. */
		myString.setChar(i,grape); /* Can also be setChar(i,"🍇") */
	}
}

myString.log(); /* Prints out the newly edited String. */
```
The output will be: "🍇🍇🍇🍇🍇🍇 🍇 🍇🍇🍇 🍇🍇🍇🍇🍇🍇🍇🍇🍇🍇"

This would be nearly impossible without using [OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) or OChar.

## Breakdown
```cpp
union{
	char          asSingleByte;
	unsigned char asMultiByte[OCHARBYTES];
}get;
```
Choose which way you wish to interpret the character data.

`get.asSingleByte` Gets the character data as single-byte (first index), like an ordinary character. Only for non-Unicode Chars.

`get.asMultiByte` Gets the character data as a multi-byte, like a string. Only for Unicode Chars.
```cpp
OUnicodeType  bd[OCHARBYTES];
```
An array of [OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md) identifiers for each byte stored by this Char.
```cpp
unsigned char byteCount;
```
The count of bytes used by this Char.
```cpp
bool          isUnicode;
```
Is this Char Unicode (multi-byte)?

```cpp
inline OChar(void) : get{0}, bd{OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL}, byteCount{1}, isUnicode{0} {}
```
Empty constructor. Sets all values to their default state.
```cpp
OChar(const char*); void setTo(const char*); OChar& operator=(const char*);
```
Sets this Char's contents to the given multi-byte character(String).

Parameters:

`const char* c - The given Unicode character to set this Char to.`

```cpp
OChar(char); void setTo(char); OChar& operator=(char);
```
Sets this Char's contents to the given single-byte character.

Parameters:

`char c - The given ASCII character to set this Char to.`

```cpp
void clear(void);
```
Clears this Char's contents.
```cpp
inline operator char(void)       { return get.asSingleByte; }
```
Converts this Char into a standard single-byte character, or the first byte of the multi-byte character if this is Unicode.
```cpp
inline operator const char*(void){ return ( (isUnicode) ? (const char*)get.asMultiByte : 0 ); }
```
Converts this Char into a String, or nothing if this is not Unicode.

```cpp
virtual void log(bool verbose=false, bool newLine=true) override;
```
Logs this Char's contents out to the terminal.

Parameters:

`bool verbose - Log verbose information (such as byte and bit information) instead of the character itself. Default is false.`

`bool newLine - Should the output be placed on a newline or append to the current one if applicable? Default is true.`

## Other Information
As was stated previously, OChar should **NOT** be used for creating character arrays(Strings).
```cpp
OChar* myString;      /* BAD! */
const char* myString; /* CORRECT! */
OString myString;     /* BEST! */
```
It is highly recommended to use [OStrings](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) instead of a traditional character array, as [OStrings](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) provide
full support for Unicode characters, among many other things.

Review the documentation for [OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md) for more information.

#### See Also:
[OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)

[OUnicodeType](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OUnicodeType.md)