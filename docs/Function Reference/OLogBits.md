# Function Reference: OLogBits
## Binary Diagnostics
Logs the individual bits of the given amount of bytes at the given memory address. 

## Declarations
```cpp
extern void OLogBits(const void* data, size_t bytes, bool newLine=true);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OLog.hpp]
[Defined  : src/OSL/OLog.cpp]
```

### Parameters
`const void* data` - A pointer to the memory address to index and log.

`size_t bytes` - The number of bytes to index for logging.

`bool newLine` - Should the output be placed on a newline or append to the current one? Default is true.

## Use
OLogBits is used to print out the [binary information](https://en.wikipedia.org/wiki/Binary_code) located at the given memory address.

As an example, take the number `299,792,458`, better known as the [Speed of Light](https://en.wikipedia.org/wiki/Speed_of_light).
In C/C++, this number would be expressed as an [integer](https://www.tutorialspoint.com/cprogramming/c_data_types.htm#integer-types).

An integer, or `int`, is comprised of 4 bytes:
```
[299792458]------------+
                       |
                       |
 [01001010], [01111000], [11011110], [00010001]
   Byte 1      Byte 2      Byte 3      Byte 4
```
Visualising the binary information like this can be incredibly useful, especially if you're working with [bitwise operators](https://en.wikipedia.org/wiki/Bitwise_operations_in_C).
This is where OLogBits comes in.

In order to visualise binary information, you must provide a [memory address](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) to the data you're trying to visualise,
and the amount of bytes to read from the given memory address.

The following code prints out the binary information at the memory address of a given value:
```cpp
int myData=299792458;
OLogBits(&myData, sizeof(myData));
```
The output will be `[ 01001010 01111000 11011110 00010001 ]`.

Note how the `sizeof` operator is used as the second parameter. This is because an `int` in C is (usually) 4 bytes,
therefore we need to tell OLogBits how many bytes to index at the memory address.

Do **NOT** attempt to use this on a [heap-allocated](https://www.geeksforgeeks.org/stack-vs-heap-memory-allocation/) array! This will NOT work:
```cpp
const char* myData="This is an array of characters";
/* myData is already a pointer, no need to retrieve its address. */
OLogBits(myData, sizeof(myData));
```
This does NOT work correctly. It may appear to work on smaller arrays, however there is a flaw in the syntax.

The second parameter is `sizeof(myData)`. This is incorrect, as this retrieves [the size of the pointer](https://stackoverflow.com/questions/17298172/how-does-sizeof-work-for-char-pointer-variables), instead of the size of the array's contents.

For Strings, use the [OStringLength](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OStringLength.md) function:
```cpp
const char* myData="This is an array of characters";
/* myData is already a pointer, no need to retrieve its address. */
OLogBits(myData, OStringLength(myData));
```
This now works as intended.

Stack allocated arrays, however, can be used with `sizeof`:
```cpp
int myData[2]={ 299792458, 854297992 };
/* myData is already a pointer, no need to retrieve its address. */
OLogBits(myData, sizeof(myData));
```

## Other Information
Alongside being able to log the information out to the terminal, there is an optional third parameter that decides whether the output should be placed on a newline, or extends the current line.

By default, it will always place the output on a newline:
```cpp
int myData=299792458;
int myOtherData=854297992;
OLogBits(&myData, sizeof(myData));
OLogBits(&myData, sizeof(myOtherData));
```
The output will be:
```
[ 01001010 01111000 11011110 00010001 ]
[ 01001010 01111000 11011110 00010001 ]
```
However, by passing `false` as the third parameter, it will instead extend the current line:
```cpp
int myData=299792458;
int myOtherData=854297992;
OLogBits(&myData, sizeof(myData),false);
OLogBits(&myData, sizeof(myOtherData),false);
OLog("\n"); /* Make sure to close off the newline. */
```
This time, the output will be `[ 01001010 01111000 11011110 00010001 ] [ 01001010 01111000 11011110 00010001 ]`.

#### See Also:
[OLog](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OLog.md)