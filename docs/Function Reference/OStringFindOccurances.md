# Function Reference: OStringFindOccurances
## Occurance Checking
Returns the count of times the substring is found in the given String.

## Declaration
```cpp
extern size_t OStringFindOccurances(const char* string, const char* substring);
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
The number of times the substring was found in the given String.

## Use
OStringFindOccurances is used for counting how many times a substring is found within a String
Note that it does **NOT** return the indicies of those substrings, only the count of substrings found.

Take the example String `"I love Grapes. Grapes are delicious"`. The substring `"Grapes"` has `2` occurances,
and the substring `"e"` has `5` occurances.

To retrieve the number of occurances of a given substring, use the following syntax:
```cpp
size_t myOccurances=OStringFindOccurances("I love Grapes. Grapes are delicious","Grapes");
```
The result will be `2`.

### Conditional Operation
Even though OStringFindOccurances returns a `0` if no occurances are found, it is not the optimal function to use when attempting to check if a substring exists in a String.
It is only to be used for counting the number of occurances.

If you want to check if a substring exists in a String, and not count the occurances, use [OStringFindFirst](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindFirst.md).

#### Why use OStringFindFirst?
[OStringFindFirst](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindFirst.md) is more efficient to use than OStringFindOccurances for merely checking if a substring exists.

The reason why is because OStringFindFirst stops searching through the String if it encounters one full occurance of the substring,
whereas	OStringFindOccurances won't stop until it goes through the entire String, which can be very computationally expensive on a large String.

#### See Also:
[OString](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OString.md)

[OStringCompare](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringCompare.md)

[OStringFindFirst](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindFirst.md)

[OStringFindLast](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringFindLast.md)

[OStringLength](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringLength.md)