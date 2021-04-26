# Function Reference: OStringFindLast
## Occurance Checking
Returns the starting index of the last occurance of the substring in the given String.

## Declaration
```cpp
extern size_t OStringFindLast(const char* string, const char* substring);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : src/OSL/OString.cpp]
```

### Parameters
`const char* string` - The String to attempt to search the substring in.

`const char* substring` - The substring to search for in the given String.

### Returns
If the substring could be found, the index of the starting position will be returned.
Otherwise, `OSTRING_NOTFOUND` will be returned, and an evaluation must be done.

## Use
OStringFindLast is used in the same way [OStringFindFirst](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindFirst.md) is, however with one crucial difference.
OStringFindFirst returns the index of *first* occurance of a given substring, whereas OStringFindLast returns the *last* occurance of a given substring. The substring can be of any length.

Take the example String `"I love Grapes. Grapes are amazing."`. The first occurance of the substring `"Grapes"` would be at index `7`,
whereas the last occurance of `"Grapes"` would be at index `15`:
```cpp
size_t myIndex=OStringFindLast("I love Grapes. Grapes are amazing.","Grapes");
```
The result will be `15`.

### Conditional Operation
Just like with [OStringFindFirst](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindFirst.md), the result will (almost) always be a positive number,
and extra steps need to be done in order to perform a conditional check if an occurance was found.


Unlike with [OStringCompare](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringCompare.md), a simple `if()` on the result will not suffice:
```cpp
size_t myIndex=OStringFindLast("I love Grapes. Grapes are amazing.","Bananas");

if(myIndex){
	OLog("Substring Found!\n");
}else{
	OLog("Substring NOT Found!\n");
}
```
In this example, `"Substring Found!"` will be printed to the terminal, despite `"Bananas"` clearly not being present in the original String.

This problem gets even worse when attempting to find a substring at the start of the main String:
```cpp
size_t myIndex=OStringFindLast(I love Grapes. Grapes are amazing.","I");

if(myIndex){
	OLog("Substring Found!\n");
}else{
	OLog("Substring NOT Found!\n");
}
```
Despite `"I"` clearly being in the String, `"Substring NOT Found!"` will be printed, since `"I"` is at the very start of the String, making its index `0`.

This is why instead of merely checking the returned value in a simple `if()` statement, you must compare it to the `OSTRING_NOTFOUND` [macro](https://www.educba.com/macros-in-c/):
```cpp
size_t myIndex=OStringFindLast("I love Grapes. Grapes are amazing.","I");

if( myIndex!=OSTRING_NOTFOUND ){
	OLog("Substring Found!\n");
}else{
	OLog("Substring NOT Found!\n");
}
```
Now this will correctly print `"Substring Found!"`, as `"I"` is a part of the String.

## Other Information
`OSTRING_NOTFOUND` is defined as `ULONG_MAX`, which is part of [<limits.h>](https://cplusplus.com/reference/climits/).

#### See Also:
[OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)

[OStringCompare](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringCompare.md)

[OStringFindFirst](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindFirst.md)

[OStringFindOccurances](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindOccurances.md)

[OStringLength](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringLength.md)