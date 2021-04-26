# Function Reference: OStringFindFirst
## Occurance Checking
Returns the starting index of the first occurance of the substring in the given String.

## Declaration
```cpp
extern size_t OStringFindFirst(const char* string, const char* substring);
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
OStringFindFirst is used for finding the index of the first occurance of a substring. The substring can be of any length.

Take the example String `"Hello World!"`.
If our substring to find was `"World!"`, the index of `"World!"`'s first occurance would be at position `6` (since arrays count from 0):
```cpp
size_t myIndex=OStringFindFirst("Hello World!","World!");
```
The result will be `6`.

If, however, we attempt to check for a substring that is not present in the main String, we will get a different result:
```cpp
size_t myIndex=OStringFindFirst("Hello World!","Earth!");
```
`"Earth!"` is not present in `"Hello World!"`, therefore `myIndex` will be set to `OSTRING_NOTFOUND`.

### Conditional Operation
Because the result will (almost) always be a positive number, extra steps need to be done in order to perform a conditional check if an occurance was found.

Unlike with [OStringCompare](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringCompare.md), a simple `if()` on the result will not suffice:
```cpp
size_t myIndex=OStringFindFirst("Hello World!","Earth!");

if(myIndex){
	OLog("Substring Found!\n");
}else{
	OLog("Substring NOT Found!\n");
}
```
In this example, `"Substring Found!"` will be printed to the terminal, despite `"Earth!"` clearly not being present in the original String.

This problem gets even worse when attempting to find a substring at the start of the main String:
```cpp
size_t myIndex=OStringFindFirst("Hello World!","Hello");

if(myIndex){
	OLog("Substring Found!\n");
}else{
	OLog("Substring NOT Found!\n");
}
```
Despite `"Hello"` clearly being in the String, `"Substring NOT Found!"` will be printed, since `"Hello"` is at the very start of the String, making its index `0`.

This is why instead of merely checking the returned value in a simple `if()` statement, you must compare it to the `OSTRING_NOTFOUND` [macro](https://www.educba.com/macros-in-c/):
```cpp
size_t myIndex=OStringFindFirst("Hello World!","Hello");

if( myIndex!=OSTRING_NOTFOUND ){
	OLog("Substring Found!\n");
}else{
	OLog("Substring NOT Found!\n");
}
```
Now this will correctly print `"Substring Found!"`, as `"Hello"` is a part of the String.

## Other Information
`OSTRING_NOTFOUND` is defined as `ULONG_MAX`, which is part of [<limits.h>](https://cplusplus.com/reference/climits/).

#### See Also:
[OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)

[OStringCompare](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringCompare.md)

[OStringFindLast](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindLast.md)

[OStringFindOccurances](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindOccurances.md)

[OStringLength](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringLength.md)