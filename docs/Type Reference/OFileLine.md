# Type Reference: OFileLine
## Small struct for a given Line of a File
OFileLine is a small wrapper for a specific Line of a File.
It's not much different from storing just a character array, the only difference is this stores the length alongside the text.

## Expansion
```cpp
struct OFileLine{
	size_t length;
	unsigned char* str;

	operator bool(void) const;
	char operator [](size_t) const;
	operator char*(void) const;
};
```
```
[Altname  : fileline_t]
[Library  : OSL]
[Declared : src/include/OSL/OFile.hpp]
[Defined  : src/OSL/OFile.cpp]
```

## Use
OFileLine is almost never used directly, as it can be [implicitly converted](https://www.tutorialspoint.com/type-conversion-in-cplusplus) to a standard character array when retrieved from `getLine()`.

For more information on how OFileLine is retrieved, see the documentation for [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md).

To store an OFileLine, use the following syntax:
```cpp
/* myFile is defined elsewhere, it is an OFile. */
OFileLine myLine=myFile.getLine(0);
```
Note that [OFiles](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md) do NOT return an OFileLine when using array notation:
```cpp
/* THIS WILL NOT WORK! */
OFileLine myLine=myLine[0]; /* <- Returns a char*, NOT an OFileLine! */
```
When attempting to input an OFileLine as a parameter into a [variadic function](https://en.wikipedia.org/wiki/Variadic_function), such as `OLog()`,
you should *always* send the contents of the OFileLine instead of the OFileLine itself:
```cpp
OLog("%s\n",myLine.str);
```

To directly read a character from an OFileLine, you can use array notation instead of directly reading the internal character array:
```cpp
for(size_t i=0;i<myLine.length;i++){
	OLog("%c",myLine[i]); /* Could also do myLine.str[i] */
}
OLog("\n");
```

## Breakdown
```cpp
size_t length;
```
The length of this Line.
```cpp
unsigned char* str;
```
The string stored in this Line.
```cpp
operator bool(void) const;
```
Allows for if() checking on this Line.
```cpp
char operator [](size_t) const;
```
Allows for getting a character from a given index without having to use the "str" member.
```cpp
operator char*(void) const;
```
Allows for converting to a string.

## Other Information
OFileLines are rarely ever used directly, since they are almost exclusively used internally.
In most cases it's preferred you retrieve the line as an [OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)
instead of an OFileLine, since it provides Unicode support.

#### See Also:
[OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)

[OFileContent](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileContent.md)